#FPS_Journey

Goals For this project:
 * Clean the Code up, and optimze it as well 
    * Clean up the Entity-Component-System (I think using a hash table for the components might work better, and try to use pointers to
         reference the original entity ID, or its fine, since I am only using a unsigned int)
   * Also I want to make it where I can import information from a text file or xml about a zombie or wall and it has all the data for it.
 * optimize the collision system
    * Make a BSP tree to partition the space in to "rooms" where the player will be to only do collision in that room (since my game will by tight corridors (and also partition so that we only need to render what the player sees and in a specific order)
    * Hill Climbing for the support functions of the Convex regions
    * Better Grid system (Need to experiemnt with different ideas for now) (once the BSP tree works, use the grid inside of the partiton only, tada, way better performance)
    * Account for different kinds of objects
        * Spheres are good for certain objects while AABB are better (depends on the situation) I like spheres for the orientation aspect, 
        * but the AABB is fine, just map to one local coordinate system if needed (I already do this for the GJK Algorthim)
    * Need to make sure that flat planes (the Walls) can not be tunneled as easily
 * Physics system
      
 * Animation
    * Make a skeleton class, joint class,
    * a way to where we can put a model on to a skeleton and change the position of the vertices based on the skeleton (easy math, just implement it)
    * build a engine to build models (or use blender and import it in)  and use inverse/forward kinematics to move the skeleton
  
