#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_RIGHT_HANDED

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/string_cast.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <map>
#include <mutex>

#include <visii/utilities/static_factory.h>
#include <visii/transform_struct.h>

using namespace glm;
using namespace std;

/**
 * The "Transform" component places an entity into the scene.
 * These transform components represent a scale, a rotation, and a translation, in that order.
*/
class Transform : public StaticFactory
{
    friend class StaticFactory;
    friend class Entity;
  private:
    /* Scene graph information */
    int32_t parent = -1;
	  std::set<int32_t> children;

    /* Local <=> Parent */
    vec3 scale = vec3(1.0);
    vec3 position = vec3(0.0);
    quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);

    vec3 linearVelocity = vec3(0.0);
    quat angularVelocity = quat(1.f,0.f,0.f,0.f);
    vec3 scalarVelocity = vec3(0.0);

    vec3 right = vec3(1.0, 0.0, 0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 forward = vec3(0.0, 0.0, 1.0);

    mat4 localToParentTransform = mat4(1);
    mat4 localToParentRotation = mat4(1);
    mat4 localToParentTranslation = mat4(1);
    mat4 localToParentScale = mat4(1);

    mat4 parentToLocalTransform = mat4(1);
    mat4 parentToLocalRotation = mat4(1);
    mat4 parentToLocalTranslation = mat4(1);
    mat4 parentToLocalScale = mat4(1);

    mat4 localToParentMatrix = mat4(1);
    mat4 parentToLocalMatrix = mat4(1);

    mat4 nextLocalToParentTranslation = mat4(1);
    mat4 nextLocalToParentRotation = mat4(1);
    mat4 nextLocalToParentScale = mat4(1);

    mat4 nextParentToLocalTranslation = mat4(1);
    mat4 nextParentToLocalRotation = mat4(1);
    mat4 nextParentToLocalScale = mat4(1);

    mat4 nextLocalToParentMatrix = mat4(1);
    mat4 nextParentToLocalMatrix = mat4(1);

    /* Local <=> World */
    mat4 localToWorldMatrix = mat4(1);
    mat4 worldToLocalMatrix = mat4(1);
    mat4 nextLocalToWorldMatrix = mat4(1);
    mat4 nextWorldToLocalMatrix = mat4(1);

    // from local to world decomposition. 
    // May only approximate the localToWorldMatrix
    glm::vec3 worldScale;
    glm::quat worldRotation;
    glm::vec3 worldTranslation;
    glm::vec3 worldSkew;
    glm::vec4 worldPerspective;
    
    // float interpolation = 1.0;

    /* TODO */
	static std::shared_ptr<std::mutex> editMutex;
    static bool factoryInitialized;

    static Transform transforms[MAX_TRANSFORMS];
    static TransformStruct transformStructs[MAX_TRANSFORMS];
    static std::map<std::string, uint32_t> lookupTable;
    
    /* Updates cached rotation values */
    void updateRotation();

    /* Updates cached position values */
    void updatePosition();

    /* Updates cached scale values */
    void updateScale();

    /* Updates cached final local to parent matrix values */
    void updateMatrix();

    /* Updates cached final local to world matrix values */
    void updateWorldMatrix();

    /* updates all childrens cached final local to world matrix values */
    void updateChildren();

    /* updates the struct for this transform which can be uploaded to the GPU. */
    void updateStruct();
    
    /* traverses from the current transform up through its ancestors, 
    computing a final world to local matrix */
    glm::mat4 computeWorldToLocalMatrix();
    glm::mat4 computeNextWorldToLocalMatrix();

    Transform();
    Transform(std::string name, uint32_t id);

    /* Indicates that one of the components has been edited */
    static bool anyDirty;

    /* Indicates this component has been edited */
    bool dirty = true;

  public:
    /**
     * Constructs a transform with the given name.
     * 
     * @param name A unique name for this transform.
     * @param scale The initial scale of the transform, applied first. 
     * @param rotation The initial scale of the transform, applied after scale.
     * @param position The initial position of the transform, applied after rotation.
     * @returns a reference to a transform component
    */
    static Transform* create(std::string name, 
      vec3 scale = vec3(1.0f), 
      quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f),
      vec3 position = vec3(0.f) 
    );

    /** 
     * @param name The name of the transform to get
     * @returns a transform who's name matches the given name 
     */
    static Transform* get(std::string name);

    /** @returns a pointer to the table of TransformStructs required for rendering*/
    static TransformStruct* getFrontStruct();

    /** @returns a pointer to the table of transform components */
    static Transform* getFront();

    /** @returns the number of allocated transforms */
	  static uint32_t getCount();

    /** @returns the name of this component */
	  std::string getName();

    /** @returns A map whose key is a transform name and whose value is the ID for that transform */
	  static std::map<std::string, uint32_t> getNameToIdMap();

    /** @param name The name of the transform to remove */
    static void remove(std::string name);

    /** Allocates the tables used to store all transform components */
    static void initializeFactory();

    /** @returns True if the tables used to store all transform components have been allocated, and False otherwise */
    static bool isFactoryInitialized();

    /** @returns True the current transform is a valid, initialized transform, and False if the transform was cleared or removed. */
	  bool isInitialized();

    /** Iterates through all transform components, computing transform metadata for rendering purposes. */
    static void updateComponents();

    /** Clears any existing transform components. */
    static void clearAll();

    /** @return True if this transform has been modified since the previous frame, and False otherwise */
	  bool isDirty() { return dirty; }

    /** @return True if any the transform has been modified since the previous frame, and False otherwise */
	  static bool areAnyDirty();

    /** @return True if the Transform has not been modified since the previous frame, and False otherwise */
	  bool isClean() { return !dirty; }

    /** Tags the current component as being modified since the previous frame. */
	  void markDirty();

    /** Tags the current component as being unmodified since the previous frame. */
	  void markClean() { dirty = false; }

    /** For internal use. Returns the mutex used to lock transforms for processing by the renderer. */
    static std::shared_ptr<std::mutex> getEditMutex();

    /** @returns a json string representation of the current component */
    std::string toString();

    /** 
     *  Transforms direction from local to parent.
     *  This operation is not affected by scale or position of the transform.
     *  The returned vector has the same length as the input direction.
     *  
     *  @param direction The direction to apply the transform to.
     *  @return The transformed direction.
    */
    vec3 transformDirection(vec3 direction);

    /** 
     * Transforms position from local to parent. Note, affected by scale.
     * The opposite conversion, from parent to local, can be done with Transform.inverse_transform_point
     * 
     * @param point The point to apply the transform to.
     * @return The transformed point. 
    */
    vec3 transformPoint(vec3 point);

    /** 
     * Transforms vector from local to parent.
     * This is not affected by position of the transform, but is affected by scale.
     * The returned vector may have a different length that the input vector.
     * 
     * @param vector The vector to apply the transform to.
     * @return The transformed vector.
    */
    vec3 transformVector(vec3 vector);

    /** 
     * Transforms a direction from parent space to local space.
     * The opposite of Transform.transform_direction.
     * This operation is unaffected by scale.
     * 
     * @param point The direction to apply the inverse transform to.
     * @return The transformed direction.
    */
    vec3 inverseTransformDirection(vec3 direction);

    /** 
     * Transforms position from parent space to local space.
     * Essentially the opposite of Transform.transform_point.
     * Note, affected by scale.
     * 
     * @param point The point to apply the inverse transform to.
     * @return The transformed point.
    */
    vec3 inverseTransformPoint(vec3 point);

    /** 
     * Transforms a vector from parent space to local space.
     * The opposite of Transform.transform_vector.
     * This operation is affected by scale.
     * 
     * @param point The vector to apply the inverse transform to.
     * @return The transformed vector.
    */
    vec3 inverseTransformVector(vec3 vector);

    /**
     *  Rotates the transform so the forward vector points at the target's current position.
     *  Then it rotates the transform to point its up direction vector in the direction hinted at 
     *  by the parentUp vector.
     * 
     * @param at The position to point the transform towards
     * @param up The unit direction pointing upwards
     * @param eye (optional) The position to place the object
    */
    void lookAt(vec3 at, vec3 up, vec3 eye = vec3(NAN));

    // /**
    // Applies a rotation of eulerAngles.z degrees around the z axis, eulerAngles.x degrees around 
    // the x axis, and eulerAngles.y degrees around the y axis (in that order).
    // If relativeTo is not specified, rotation is relative to local space.
    // */
    // void rotate(vec3 eularAngles, Space = Space::Local);

    // /** 
    //  * Rotates the transform about the provided axis, passing through the provided point in parent 
    //  * coordinates by the provided angle in degrees.
    //  * This modifies both the position and rotation of the transform.
    //  * 
    //  * @param point The pivot point in space to rotate around.
    //  * @param angle The angle (in radians) to rotate.
    //  * @param axis  The axis to rotate about.
    // */
    // void rotateAround(vec3 point, float angle, vec3 axis);

    /** 
     * Rotates the transform through the provided quaternion, passing through the provided point in parent 
     * coordinates.
     * This modifies both the position and rotation of the transform.
     * 
     * @param point The pivot point in space to rotate around.
     * @param quaternion   The quaternion to use for rotation.
    */
    void rotateAround(vec3 point, glm::quat quaternion);

    /** 
     * Sets an optional additional transform, useful for representing normally unsupported transformations
     * like sheers and projections. 
     * 
     * @param transformation  a 4 by 4 column major transformation matrix
     * @param decompose       attempts to use singular value decomposition to decompose the provided transform into a translation, rotation, and scale 
    */
    void setTransform(glm::mat4 transformation, bool decompose = true);

    /** @return A quaternion rotating the transform from local to parent */
    quat getRotation();

    /** 
     * Sets the rotation of the transform from local to parent via a quaternion 
     * 
     * @param newRotation The new rotation quaternion to set the current transform quaternion to.
    */
    void setRotation(quat newRotation);

    // /** 
    //  * Sets the rotation of the transform from local to parent using an axis 
    //  * in local space to rotate about, and an angle in radians to drive the rotation. 
    //  * 
    //  * @param angle The angle (in radians) to rotate.
    //  * @param axis  The axis to rotate about.
    // */
    // void setRotation(float angle, vec3 axis);

    /** 
     * Adds a rotation to the existing transform rotation from local to parent 
     * via a quaternion. 
     * 
     * @param additionalRotation The rotation quaternion apply to the existing transform quaternion.
    */
    void addRotation(quat additionalRotation);

    // /** 
    //  * Adds a rotation to the existing transform rotation from local to parent 
    //  * using an axis in local space to rotate about, and an angle in radians to 
    //  * drive the rotation
    //  *  
    //  * @param angle The angle (in radians) to rotate the current transform quaterion by.
    //  * @param axis  The axis to rotate about.
    // */
    // void addRotation(float angle, vec3 axis);

    /** @returns a position vector describing where this transform will be translated to in its parent space. */
    vec3 getPosition();

    /** @returns a vector pointing right relative to the current transform placed in its parent's space. */
    vec3 getRight();

    /** @returns a vector pointing up relative to the current transform placed in its parent's space. */
    vec3 getUp();

    /** @returns a vector pointing forward relative to the current transform placed in its parent's space. */
    vec3 getForward();

    /** 
     * Sets the position vector describing where this transform should be translated to when placed in its 
     * parent space. 
     * 
     * @param newPosition The new position to set the current transform position to.
    */
    void setPosition(vec3 newPosition);

    /** 
     * Adds to the current the position vector describing where this transform should be translated to 
     * when placed in its parent space. 
     * 
     * @param additionalPosition The position (interpreted as a vector) to add onto the current transform position.
    */
    void addPosition(vec3 additionalPosition);

    // /**
    //  * Sets the position vector describing where this transform should be translated to when placed in its 
    //  * parent space. 
    //  * 
    //  * @param x The x component of the new position.
    //  * @param y The y component of the new position.
    //  * @param z The z component of the new position.
    // */
    // void setPosition(float x, float y, float z);

    // /**
    //  * Adds to the current the position vector describing where this transform should be translated to 
    //  * when placed in its parent space. 
    //  * 
    //  * @param dx The change in x to add onto the current transform position.
    //  * @param dy The change in y to add onto the current transform position.
    //  * @param dz The change in z to add onto the current transform position.
    // */
    // void addPosition(float dx, float dy, float dz);

    /** 
     * @returns the scale of this transform from local to parent space along its right, up, and forward 
     * directions respectively 
     */
    vec3 getScale();

    /** 
     * Sets the scale of this transform from local to parent space along its right, up, and forward 
     * directions respectively. 
     * 
     * @param newScale The new scale to set the current transform scale to.
    */
    void setScale(vec3 newScale);

    // /** 
    //  * Sets the scale of this transform from local to parent space along its right, up, and forward 
    //  * directions simultaneously.
    //  * 
    //  * @param newScale The new uniform scale to set the current transform scale to.
    // */
    // void setScale(float newScale);

    /** 
     * Adds to the current the scale of this transform from local to parent space along its right, up, 
     * and forward directions respectively 
     * 
     * @param additionalScale The scale to add onto the current transform scale.
    */
    void addScale(vec3 additionalScale);

    // /** 
    //  * Sets the scale of this transform from local to parent space along its right, up, and forward 
    //  * directions respectively.
    //  * 
    //  * @param x The x component of the new scale.
    //  * @param y The y component of the new scale.
    //  * @param z The z component of the new scale.
    // */
    // void setScale(float x, float y, float z);

    // /** 
    //  * Adds to the current the scale of this transform from local to parent space along its right, up, 
    //  * and forward directions respectively 
    //  * 
    //  * @param dx The change in x to add onto the current transform scale.
    //  * @param dy The change in y to add onto the current transform scale.
    //  * @param dz The change in z to add onto the current transform scale.
    // */
    // void addScale(float dx, float dy, float dz);

    // /** 
    //  * Adds to the scale of this transform from local to parent space along its right, up, and forward 
    //  * directions simultaneously 
    //  * 
    //  * @param ds The change in scale to uniformly add onto all components of the current transform scale.
    // */
    // void addScale(float ds);

    /** 
     * Sets the linear velocity vector describing how fast this transform is translating within its 
     * parent space. Causes motion blur.
     * 
     * @param velocity The new linear velocity to set the current transform linear velocity to, in meters per second.
     * @param frames_per_second Used to convert meters per second into meters per frame. Useful for animations.
    */
    void setLinearVelocity(vec3 velocity, float frames_per_second = 1.0f, float mix = 0.0f);

    /** 
     * Sets the angular velocity vector describing how fast this transform is rotating within its 
     * parent space. Causes motion blur.
     * 
     * @param velocity The new angular velocity to set the current transform angular velocity to, in radians per second.
     * @param frames_per_second Used to convert radians per second into scale per frame. Useful for animations.
    */
    void setAngularVelocity(quat velocity, float frames_per_second = 1.0f, float mix = 0.0f);

    /** 
     * Sets the scalar velocity vector describing how fast this transform is scaling within its 
     * parent space. Causes motion blur.
     * 
     * @param velocity The new scalar velocity to set the current transform scalar velocity to, in additional scale per second
     * @param frames_per_second Used to convert additional scale per second into additional scale per frame. Useful for animations.
    */
    void setScalarVelocity(vec3 velocity, float frames_per_second = 1.0f, float mix = 0.0f);

    /** 
     * @returns the final matrix transforming this object from it's parent coordinate space to it's 
     * local coordinate space 
     */
    glm::mat4 getParentToLocalMatrix();

    /** 
     * @returns the final matrix transforming this object from it's parent coordinate space to it's 
     * local coordinate space, accounting for linear and angular velocities.
     */
    glm::mat4 getNextParentToLocalMatrix();

    /** 
     * @returns the final matrix transforming this object from it's local coordinate space to it's 
     * parents coordinate space 
    */
    glm::mat4 getLocalToParentMatrix();

    /** 
     * @returns the final matrix transforming this object from it's local coordinate space to it's 
     * parents coordinate space, accounting for linear and angular velocities.
    */
    glm::mat4 getNextLocalToParentMatrix();

    /** 
     * @returns the final matrix translating this object from it's local coordinate space to it's 
     * parent coordinate space 
     */
    glm::mat4 getLocalToParentTranslationMatrix();

    /** 
     * @returns the final matrix translating this object from it's local coordinate space to it's 
     * parent coordinate space 
     */
    glm::mat4 getLocalToParentScaleMatrix();

    /** 
     * @returns the final matrix rotating this object in it's local coordinate space to it's 
     * parent coordinate space 
     */
    glm::mat4 getLocalToParentRotationMatrix();

    /** 
     * @returns the final matrix translating this object from it's parent coordinate space to it's 
     * local coordinate space 
     */
    glm::mat4 getParentToLocalTranslationMatrix();

    /** 
     * @returns the final matrix scaling this object from it's parent coordinate space to it's 
     * local coordinate space 
     */
    glm::mat4 getParentToLocalScaleMatrix();

    /** 
     * @returns the final matrix rotating this object from it's parent coordinate space to it's 
     * local coordinate space 
    * */
    glm::mat4 getParentToLocalRotationMatrix();

    /** 
     * Set the parent of this transform, whose transformation will be applied after the current
     * transform. 
     * 
     * @param parent The transform component to constrain the current transform to. Any existing parent constraint is replaced.
    */
    void setParent(Transform * parent);

    /** Removes the parent-child relationship affecting this node. */
    void clearParent();

    /** 
     * Add a child to this transform, whose transformation will be applied before the current
     * transform. 
     * 
     * @param child The child transform component to constrain to the current transform. Any existing parent constraint is replaced.
    */
	  void addChild(Transform*  child);

    /** 
     * Removes a child transform previously added to the current transform. 
     * 
     * @param child The constrained child transform component to un-constrain from the current transform. Any existing parent constraint is replaced.
    */
	  void removeChild(Transform* child);

    /** 
     * @returns a matrix transforming this component from world space to its local space, taking all 
     * parent transforms into account. 
     */
    glm::mat4 getWorldToLocalMatrix();

    /** 
     * @returns a matrix transforming this component from its local space to world space, taking all 
     * parent transforms into account. 
     */
	  glm::mat4 getLocalToWorldMatrix();

    /** 
     * @returns a matrix transforming this component from its local space to world space, taking all 
     * parent transforms into account, in addition to any linear and angular velocities. 
     */
	  glm::mat4 getNextLocalToWorldMatrix();

    /** 
     * @returns a (possibly approximate) scale scaling the current transform from 
       * local space to world space, taking all parent transforms into account 
       */
    glm::vec3 getWorldScale();

    /** 
     * @returns a (possibly approximate) rotation rotating the current transform from 
     * local space to world space, taking all parent transforms into account 
     */
	  glm::quat getWorldRotation();

    /** 
     * @returns a (possibly approximate) translation moving the current transform from 
     * local space to world space, taking all parent transforms into account 
     */
    glm::vec3 getWorldTranslation();

    /** 
     * @returns a (possibly approximate) rotation matrix rotating the current transform from 
     * local space to world space, taking all parent transforms into account 
     */
    glm::mat4 getWorldToLocalRotationMatrix();

    /** 
     * @returns a (possibly approximate) rotation matrix rotating the current transform from 
     * world space to local space, taking all parent transforms into account 
     */
    glm::mat4 getLocalToWorldRotationMatrix();

    /** 
     * @returns a (possibly approximate) translation matrix translating the current transform from 
     * local space to world space, taking all parent transforms into account 
     */
    glm::mat4 getWorldToLocalTranslationMatrix();

    /** 
     * @returns a (possibly approximate) translation matrix rotating the current transform from 
     * world space to local space 
     */
    glm::mat4 getLocalToWorldTranslationMatrix();

    /** 
     * @returns a (possibly approximate) scale matrix scaling the current transform from 
     * local space to world space, taking all parent transforms into account 
     */
    glm::mat4 getWorldToLocalScaleMatrix();

    /** 
     * @returns a (possibly approximate) scale matrix scaling the current transform from 
     * world space to local space, taking all parent transforms into account 
     */
    glm::mat4 getLocalToWorldScaleMatrix();

    /** @returns a struct with only essential data */
    TransformStruct &getStruct();
};
