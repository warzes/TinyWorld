возможно по персонажу

To avoid rotation, you can set the Сangular factorТ to zero, which disables the angular rotation effect during collisions and other constraints. See btRigidBody::setAngularFactor. Other options (that are less recommended) include setting the inverse inertia tensor to zero for the up axis, or using a angular-only hinge constraint.


¬озможно SetDamping() в ригидбоди
