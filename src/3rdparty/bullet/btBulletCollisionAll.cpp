#include "stdafx.h"

#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable : 4701)
#	pragma warning(disable : 4702)
#	pragma warning(disable : 5045)
#endif

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.cpp"
#include "BulletCollision/BroadphaseCollision/btDbvt.cpp"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp"
#include "BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp"
#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp"
#include "BulletCollision/BroadphaseCollision/btDispatcher.cpp"
#include "BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp"
#include "BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp"
#include "BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp"
#include "BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp"
#include "BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btManifoldResult.cpp"
#include "BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp"
#include "BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp"
#include "BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcherMt.cpp"
#include "BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btCollisionObject.cpp"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.cpp"
#include "BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp"
#include "BulletCollision/CollisionDispatch/btUnionFind.cpp"
#include "BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp"
#include "BulletCollision/CollisionDispatch/btGhostObject.cpp"
#include "BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp"
#include "BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp"
#include "BulletCollision/NarrowPhaseCollision/btConvexCast.cpp"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp"
#include "BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp"
#include "BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp"
#include "BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp"
#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp"
#include "BulletCollision/CollisionShapes/btBox2dShape.cpp"
#include "BulletCollision/CollisionShapes/btConvexPolyhedron.cpp"
#include "BulletCollision/CollisionShapes/btShapeHull.cpp"
#include "BulletCollision/CollisionShapes/btBoxShape.cpp"
#include "BulletCollision/CollisionShapes/btConvexShape.cpp"
#include "BulletCollision/CollisionShapes/btSphereShape.cpp"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp"
#include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.cpp"
#include "BulletCollision/CollisionShapes/btCapsuleShape.cpp"
#include "BulletCollision/CollisionShapes/btCylinderShape.cpp"
#include "BulletCollision/CollisionShapes/btStridingMeshInterface.cpp"
#include "BulletCollision/CollisionShapes/btCollisionShape.cpp"
#include "BulletCollision/CollisionShapes/btEmptyShape.cpp"
#include "BulletCollision/CollisionShapes/btTetrahedronShape.cpp"
#include "BulletCollision/CollisionShapes/btCompoundShape.cpp"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp"
#include "BulletCollision/CollisionShapes/btTriangleBuffer.cpp"
#include "BulletCollision/CollisionShapes/btConcaveShape.cpp"
#include "BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp"
#include "BulletCollision/CollisionShapes/btTriangleCallback.cpp"
#include "BulletCollision/CollisionShapes/btConeShape.cpp"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.cpp"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp"
#include "BulletCollision/CollisionShapes/btConvex2dShape.cpp"
#include "BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp"
#include "BulletCollision/CollisionShapes/btConvexHullShape.cpp"
#include "BulletCollision/CollisionShapes/btOptimizedBvh.cpp"
#include "BulletCollision/CollisionShapes/btTriangleMesh.cpp"
#include "BulletCollision/CollisionShapes/btConvexInternalShape.cpp"
#include "BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.cpp"
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp"
#include "BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp"
#include "BulletCollision/CollisionShapes/btSdfCollisionShape.cpp"
#include "BulletCollision/CollisionShapes/btMiniSDF.cpp"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.cpp"

//#include "BulletCollision/Gimpact/btContactProcessing.cpp"
//#include "BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp"
//#include "BulletCollision/Gimpact/btTriangleShapeEx.cpp"
//#include "BulletCollision/Gimpact/gim_memory.cpp"
//#include "BulletCollision/Gimpact/btGImpactBvh.cpp"
//#include "BulletCollision/Gimpact/btGImpactShape.cpp"
//#include "BulletCollision/Gimpact/gim_box_set.cpp"
//#include "BulletCollision/Gimpact/gim_tri_collision.cpp"
//#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp"
//#include "BulletCollision/Gimpact/btGenericPoolAllocator.cpp"
//#include "BulletCollision/Gimpact/gim_contact.cpp"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif