using Entix;

namespace Sandbox
{
    public class MyRotatingSphere : Entity
    {
        [SerializeField]
        private TransformComponent _transform;
        
        [SerializeField]
        private StaticMeshComponent _mesh;

        public bool isRotating = false;

        [TabGroup("Rotation parameters")]
        public float speed = 0.01;

        void OnCreate()
        {
            _transform = GetComponent<TransformComponent>();
            _mesh = GetComponent<StaticMeshComponent>();
        }

        void OnTick()
        {
            if(isRotating)
            {
                m_transform.rotation.x += speed;
            }
        }

        void OnDestroy()
        {
        }
    }
}