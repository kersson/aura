#ifndef AURA_BACKEND_CUDA_DEVICE_HPP
#define AURA_BACKEND_CUDA_DEVICE_HPP

#include <cstring>
#include <boost/move/move.hpp>
#include <cuda.h>
#include <boost/aura/backend/cuda/call.hpp>
#include <boost/aura/backend/cuda/context.hpp>
#include <boost/aura/misc/deprecate.hpp>

namespace boost
{
namespace aura {
namespace backend_detail {
namespace cuda {

/**
 * device class
 *
 * every interaction with devices starts from this class
 */
class device {

private:
  BOOST_MOVABLE_BUT_NOT_COPYABLE(device)

public:

  /// create empty device object 
  inline explicit device() : context_(nullptr) {}
   
  /**
   * create device form ordinal
   *
   * @param ordinal device number
   */
  inline explicit device(std::size_t ordinal) : 
    context_(new detail::context(ordinal)),
    ordinal_(ordinal) {}

  /// destroy device
  inline ~device() {
    finalize();
  }

  /**
   * move constructor, move device information here, invalidate other
   *
   * @param d device to move here
   */
  device(BOOST_RV_REF(device) d) : 
    context_(d.context_), ordinal_(d.ordinal_) {
    d.context_ = nullptr;
  }

  /**
   * move assignment, move device information here, invalidate other
   *
   * @param d device to move here
   */
  device& operator=(BOOST_RV_REF(device) d) 
  {
    finalize();
    context_ = d.context_;
    ordinal_ = d.ordinal_;
    d.context_ = nullptr;
    return *this;
  }

  /// make device active
  inline void set() {
    context_->set();
  }
  
  /// undo make device active
  inline void unset() {
    context_->unset();
  }

  /**
   * pin 
   *
   * disable unset, device context stays associated with current thread
   * usefull for interoperability with other libraries that use a context
   * explicitly
   */
  inline void pin() {
    context_->pin();
  }
  
  /// unpin (reenable unset)
  inline void unpin() {
    context_->unpin();
  } 

  /// access the device handle
  inline const CUdevice & get_backend_device() const {
    return context_->get_backend_device(); 
  }
  
  /// access the context handle
  inline const CUcontext & get_backend_context() const {
    return context_->get_backend_context(); 
  }
  
  /// access the context handle
  inline detail::context * get_context() {
    return context_; 
  }

  /// access the device ordinal 
  inline std::size_t get_ordinal() const {
    return ordinal_; 
  }

private:
  /// finalize object (called from dtor and move assign)
  void finalize() {
    if(nullptr != context_) {
      delete context_;
    }
  }

private:
  /// device context
  detail::context * context_; 
  
  /// device ordinal
  std::size_t ordinal_;
};
  
/**
 * get number of devices available
 *
 * @return number of devices
 */
inline std::size_t device_get_count() {
  int num_devices;
  AURA_CUDA_SAFE_CALL(cuDeviceGetCount(&num_devices));
  return (std::size_t)num_devices;
}

/// print device info to stdout
inline void print_system_info() {
  for(std::size_t n=0; n<device_get_count(); n++) {
    CUdevice device; 
    AURA_CUDA_SAFE_CALL(cuDeviceGet(&device, n));
    char device_name[400];
    AURA_CUDA_SAFE_CALL(cuDeviceGetName(device_name, 400, device)); 
    printf("%lu: %s\n", n, device_name); 
  }
}

inline void print_device_info() {
  print_system_info();
}
DEPRECATED(void print_device_info());

/// return the amount of free memory on the device
inline std::size_t device_get_free_memory(device &d) {
	std::size_t free; 
	std::size_t total;
	d.set();
	AURA_CUDA_SAFE_CALL(cuMemGetInfo(&free, &total));
	d.unset();
	return free;
}

/// return the amount of total memory on the device
inline std::size_t device_get_total_memory(device &d) {
	std::size_t free; 
	std::size_t total;
	d.set();
	AURA_CUDA_SAFE_CALL(cuMemGetInfo(&free, &total));
	d.unset();
	return total;
}

#include <boost/aura/backend/shared/device_info.hpp>

/// return the device info 
inline device_info device_get_info(device & d) {
  device_info di;
  // name and vendor
  AURA_CUDA_SAFE_CALL(cuDeviceGetName(di.name, sizeof(di.name)-1,
    d.get_backend_device()));
  strncpy(di.vendor, "Nvidia", sizeof(di.vendor)-1); 

  // mesh 
  int r=0;
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r, 
    CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X, d.get_backend_device()));
  di.max_mesh.push_back(r);
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r,
    CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y, d.get_backend_device()));
  di.max_mesh.push_back(r);
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r, 
    CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z, d.get_backend_device()));
  di.max_mesh.push_back(r);

  // bundle 
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r, 
    CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X, d.get_backend_device()));
  di.max_bundle.push_back(r);
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r, 
    CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y, d.get_backend_device()));
  di.max_bundle.push_back(r);
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r, 
    CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z, d.get_backend_device()));
  di.max_bundle.push_back(r);

  // fibers in bundle
  AURA_CUDA_SAFE_CALL(cuDeviceGetAttribute(&r, 
    CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, d.get_backend_device()));
  di.max_fibers_per_bundle = r;
  return di;
}

} // cuda
} // backend_detail
} // aura
} // boost

#endif // AURA_BACKEND_CUDA_DEVICE_HPP
