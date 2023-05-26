#include "stdafx.h"
#if defined(_MSC_VER)
#	pragma warning(push, 3)
#	pragma warning(disable : 4005)
#	pragma warning(disable : 4267)
#	pragma warning(disable : 4701)
#	pragma warning(disable : 5039)
#	pragma warning(disable : 5045)
#	pragma warning(disable : 5219)
#endif

#include "LinearMath/btAlignedAllocator.cpp"
#include "LinearMath/btGeometryUtil.cpp"
#include "LinearMath/btSerializer.cpp"
#include "LinearMath/btVector3.cpp"
#include "LinearMath/btConvexHull.cpp"
#include "LinearMath/btSerializer64.cpp"
#include "LinearMath/btConvexHullComputer.cpp"
#include "LinearMath/btQuickprof.cpp"
#include "LinearMath/btThreads.cpp"
//#include "LinearMath/TaskScheduler/btTaskScheduler.cpp"
//#include "LinearMath/TaskScheduler/btThreadSupportPosix.cpp"
//#include "LinearMath/TaskScheduler/btThreadSupportWin32.cpp"
//#include "LinearMath/btPolarDecomposition.cpp"
//#include "LinearMath/btReducedVector.cpp"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif