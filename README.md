#FPS_Journey

Goals For this project:
 * Clean the Code up, and optimze it as well 
    - Clean up the Entity-Component-System (I think using a hash table for the components might work better, and try to use pointers to
   reference the original entity ID)
   -Also I want to make it where I can import information from a text file or xml about a zombie or wall and it has all the data for it.
 *optimize the collision system
    -Hill Climbing for the support functions of the Convex regions
    -Better Grid system (Need to experiemnt with different ideas for now)
    -Account for different kinds of objects
        -Spheres are good for certain objects while AABB are better (depends on the situation) I like spheres for the orientation aspect, 
        - but the AABB is fine, just map to one local coordinate system if needed (I already do this for the GJK Algorthim)
    - Need to make sure that flat planes (the Walls) can not be tunneled as easily
 *Physics system
      
 *Animation
    -Look at Inverse/Forward Kinematics
    -Blending of Animation together
  
