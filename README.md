# nav-mesh-routing-optimizations
## goal
Create a basic navmesh / mesh model with functionality to add, remove, display, update, find quickest route

Create a basic machine learning system to compute when it is more efficent to recompute the quickest route, or to use cached data when the endgoal target is randomly moving away from the "current position" at 0.5x the speed of the "current position"

In short, I want to know "If the end target has moved x tiles from the last known cached position of it, is it more efficent to re-compute the quickest route, or to keep going to the last known position & only recompute once I arrive."

My parameter for "what is better" is going to be time in seconds to reach target.


-----------------------------------------------------

Allows the creation of a mesh layer comprised of nodes with up to 4 neighbors mesh.h

Allows displaying the mesh with different start & target positions - display.h

Allows vectorizing a meshModel by dynamically creating it in realtime - nodeMatrix.h
