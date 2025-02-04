#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "mlem.h"
#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "material.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>

// Task queue for better work distribution
class TaskQueue {
        std::queue<std::pair<int, int>> tasks;
        std::mutex mutex;
        std::condition_variable cv;
        bool done = false;

    public:
        void push(std::pair<int, int> task) {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.push(task);
            cv.notify_one();
        }

        bool pop(std::pair<int, int>& task) {
            std::unique_lock<std::mutex> lock(mutex);
            if (tasks.empty() && !done) cv.wait(lock);
            if (tasks.empty() && done) return false;
            task = tasks.front();
            tasks.pop();
            return true;
        }

        void finish() {
            std::unique_lock<std::mutex> lock(mutex);
            done = true;
            cv.notify_all();
        }
};

class camera {
    public:
        double aspect_ratio      = 1.0;  // ratio of an image width over height.
        int    image_width       = 100;  // rendered image width in pixel count.
        int    max_depth         =  10;  // maximum number of ray bounces into world.
        int    samples_per_pixel =  10;  // Count of random samples for each pixel
        int    block_size        =  64;

        double vfov = 90;                // Vertical FOV
        point3 lookfrom = point3(0,0,0); // Point camera is looking from
        point3 lookat   = point3(0,0,-1);// Point camera is looking at
        vec3   vup      = vec3(0,1,0);   // Camera - relative "up" direction

        double defocus_angle = 0;        // Variation angle of rays through each pixel.
        double focus_dist = 10;          // Distance from camera lookfrom point to plane of perfect focus


        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            std::vector<color> framebuffer(image_width * image_height);
            std::atomic<int> blocks_remaining{0};
            std::mutex cout_mutex;
            TaskQueue task_queue;

            // Calculate blocks with overlap for better cache utilization
            int num_blocks_x = (image_width + block_size - 1) / block_size;
            int num_blocks_y = (image_height + block_size - 1) / block_size;
            blocks_remaining = num_blocks_x * num_blocks_y;

            // Pre-calculate some common values
            const double inv_width = 1.0 / image_width;
            const double inv_height = 1.0 / image_height;

            // Enhanced worker function with local cache
            auto worker = [&]() {
                // Pre-allocate arrays for the entire block
                std::vector<ray> rays(block_size * block_size * samples_per_pixel);
                std::vector<color> block_colors(block_size * block_size);
                std::pair<int, int> block;

                while (task_queue.pop(block)) {
                    int block_x = block.first;
                    int block_y = block.second;
                    
                    // Calculate block boundaries
                    int start_x = block_x * block_size;
                    int start_y = block_y * block_size;
                    int end_x = std::min(start_x + block_size, image_width);
                    int end_y = std::min(start_y + block_size, image_height);
                    
                    // Process entire block
                    #pragma omp parallel for collapse(2) if(block_size >= 32)
                    for (int j = start_y; j < end_y; ++j) {
                        for (int i = start_x; i < end_x; ++i) {
                            color pixel_color(0, 0, 0);
                            int local_idx = (j - start_y) * block_size + (i - start_x);
                            
                            // Generate and process all rays for this pixel
                            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                                auto offset = sample_square();
                                auto pixel_sample = pixel00_loc + 
                                                  (i + offset.x()) * pixel_delta_u +
                                                  (j + offset.y()) * pixel_delta_v;

                                auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
                                auto ray_direction = pixel_sample - ray_origin;
                                
                                ray r(ray_origin, ray_direction);
                                pixel_color += ray_color(r, max_depth, world);
                            }
                            
                            framebuffer[j * image_width + i] = pixel_samples_scale * pixel_color;
                        }
                    }

                    // Update progress less frequently
                    if (--blocks_remaining % 5 == 0) {
                        std::lock_guard<std::mutex> lock(cout_mutex);
                        std::clog << "\rProgress: " << 
                            static_cast<int>((1.0 - (blocks_remaining / static_cast<double>(num_blocks_x * num_blocks_y))) * 100) 
                            << "% " << std::flush;
                    }
                }
            };

            // Optimize thread count based on hardware and block size
            const int num_threads = std::max(1, static_cast<int>(std::thread::hardware_concurrency() * 1.5));
            std::vector<std::thread> threads;

            // Push tasks in a more cache-friendly order
            for (int by = 0; by < num_blocks_y; ++by) {
                for (int bx = 0; bx < num_blocks_x; ++bx) {
                    task_queue.push({bx, by});
                }
            }

            // Launch threads with high priority
            threads.reserve(num_threads);
            for (int t = 0; t < num_threads; ++t) {
                threads.emplace_back(worker);
            }

            task_queue.finish();

            for (auto& thread : threads) {
                thread.join();
            }

            // Output the image
            for (int j = 0; j < image_height; ++j) {
                for (int i = 0; i < image_width; ++i) {
                    write_color(std::cout, framebuffer[j * image_width + i]);
                }
            }

            std::clog << "\nRendering complete.\n";
        }

    private:
        int     image_height;    // Rendered image height
        double  pixel_samples_scale;
        point3  center;          // Camera center
        point3  pixel00_loc;     // Locagtion of pixel 0, 0
        vec3    pixel_delta_u;   // Offset to pixel to the right
        vec3    pixel_delta_v;   // Offset to pixel bellow
        vec3    u, v, w;         // Camera frame basis vectors. 
        vec3    defocus_disk_u;  // Defocus dick horizontal radius;
        vec3    defocus_disk_v;  // Defocus dick vertical radius;

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            // Determine viewport dimensions.
            // auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            //auto viewport_height = 2 * h * focal_length;
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the u, v, w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            vec3 viewport_u = viewport_width * u;      // vec across viewport horizontal edge
            vec3 viewport_v = viewport_height * -v;     // vec down viewport vertical edge

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Calculate the camera defocus dick basis vectors.
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        color ray_color (const ray& r, int depth, const hittable& world) const { 
            hit_record rec;

            if (depth == 0) {
                return color(0,0,0);
            }

            if (world.hit(r, interval(0.0000000001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth -1, world);
                } else {
                    return color(0, 0, 0);
                }
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }

        void get_pixel_rays(int i, int j, std::vector<ray>& rays) const {
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                auto offset = sample_square();
                auto pixel_sample = pixel00_loc
                                  + ((i + offset.x()) * pixel_delta_u)
                                  + ((j + offset.y()) * pixel_delta_v);

                auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
                auto ray_direction = pixel_sample - ray_origin;

                rays[sample] = ray(ray_origin, ray_direction);
            }
        }

        ray get_ray(int i, int j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled
            // point around the pixel location i, j.

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                              + ((i + offset.x()) * pixel_delta_u)
                              + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        point3 defocus_disk_sample() const {
            // Return a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }
};

#endif
