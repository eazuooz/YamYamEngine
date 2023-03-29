struct Particle
{
    float4 position;
    float4 direction;

    float maxTime;
    float time;
    float speed;
    uint active;
    
    
};

struct ParticleShared
{
    uint gActiveCount;
};