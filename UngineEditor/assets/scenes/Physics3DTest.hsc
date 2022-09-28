Scene: Scene Name
Environment:
  AssetPath: assets\env\pink_sunrise_4k.hdr
  Light:
    Direction: [-0.787, -0.73299998, 1]
    Radiance: [1, 1, 1]
    Multiplier: 0.514999986
Entities:
  - Entity: 18306113171518048249
    TagComponent:
      Tag: Box
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [1, 0, 0, 0]
      Scale: [50, 1, 50]
    MeshComponent:
      AssetPath: D:\code\Ungine\UngineEditor\assets\meshes\Cube1m.fbx
    RigidBodyComponent:
      BodyType: 0
      Mass: 1
      IsKinematic: false
      Constraints:
        LockPositionX: false
        LockPositionY: false
        LockPositionZ: false
        LockRotationX: false
        LockRotationY: false
        LockRotationZ: false
    PhysicsMaterialComponent:
      StaticFriction: 1
      DynamicFriction: 1
      Bounciness: 0
    BoxColliderComponent:
      Offset: [0, 0, 0]
      Size: [1, 1, 1]
      IsTrigger: false
  - Entity: 5178862374589434728
    TagComponent:
      Tag: Camera
    TransformComponent:
      Position: [-21.7406311, 9.70659542, 15]
      Rotation: [0.999910355, -0.0133911213, 0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ModuleName: Example.BasicController
      StoredFields:
        - Name: Speed
          Type: 1
          Data: 12
        - Name: DistanceFromPlayer
          Type: 1
          Data: 10
    CameraComponent:
      Camera: some camera data...
      Primary: true
  - Entity: 14057422478420564497
    TagComponent:
      Tag: Player
    TransformComponent:
      Position: [-3.98769951, 1, -1.96695328e-06]
      Rotation: [1, 0, 0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ModuleName: Example.PlayerSphere
      StoredFields:
        - Name: HorizontalForce
          Type: 1
          Data: 10
        - Name: MaxSpeed
          Type: 6
          Data: [10, 10, 10]
        - Name: JumpForce
          Type: 1
          Data: 200
    MeshComponent:
      AssetPath: assets\meshes\Sphere1m.fbx
    RigidBodyComponent:
      BodyType: 1
      Mass: 1
      IsKinematic: false
      Constraints:
        LockPositionX: false
        LockPositionY: false
        LockPositionZ: false
        LockRotationX: true
        LockRotationY: true
        LockRotationZ: true
    PhysicsMaterialComponent:
      StaticFriction: 0.100000001
      DynamicFriction: 0.100000001
      Bounciness: 0.100000001
    SphereColliderComponent:
      Radius: 0.5
      IsTrigger: false
  - Entity: 10169503531257462571
    TagComponent:
      Tag: Box
    TransformComponent:
      Position: [0, 1.5, 0]
      Rotation: [1, 0, 0, 0]
      Scale: [2, 2, 2]
    MeshComponent:
      AssetPath: D:\code\Ungine\UngineEditor\assets\meshes\Cube1m.fbx
    RigidBodyComponent:
      BodyType: 1
      Mass: 0.5
      IsKinematic: false
      Constraints:
        LockPositionX: false
        LockPositionY: false
        LockPositionZ: false
        LockRotationX: false
        LockRotationY: false
        LockRotationZ: false
    PhysicsMaterialComponent:
      StaticFriction: 1
      DynamicFriction: 1
      Bounciness: 0
    BoxColliderComponent:
      Offset: [0, 0, 0]
      Size: [1, 1, 1]
      IsTrigger: false
  - Entity: 11149966982516343187
    TagComponent:
      Tag: Mesh Collider
    TransformComponent:
      Position: [-2.32141495, 1, -0.00171399117]
      Rotation: [1, 0, 0, 0]
      Scale: [1, 1, 1]
    MeshComponent:
      AssetPath: D:\code\Ungine\UngineEditor\assets\meshes\Sphere1m.fbx
    RigidBodyComponent:
      BodyType: 1
      Mass: 0.100000001
      IsKinematic: false
      Constraints:
        LockPositionX: false
        LockPositionY: false
        LockPositionZ: false
        LockRotationX: false
        LockRotationY: false
        LockRotationZ: false
    PhysicsMaterialComponent:
      StaticFriction: 1
      DynamicFriction: 1
      Bounciness: 0.100000001
    MeshColliderComponent:
      AssetPath: D:\code\Ungine\UngineEditor\assets\meshes\Sphere1m.fbx
      IsTrigger: false
