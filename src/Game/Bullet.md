�������� �� ���������

To avoid rotation, you can set the �angular factor� to zero, which disables the angular rotation effect during collisions and other constraints. See btRigidBody::setAngularFactor. Other options (that are less recommended) include setting the inverse inertia tensor to zero for the up axis, or using a angular-only hinge constraint.


�������� SetDamping() � ���������
