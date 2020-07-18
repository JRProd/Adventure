// #include "Debris.hpp"
// 
// #include "Engine/Core/EngineCommon.hpp"
// #include "Engine/Core/Time.hpp"
// #include "Engine/Core/Vertex_PCU.hpp"
// #include "Engine/Core/Math/MathUtils.hpp"
// #include "Engine/Core/Math/RandomNumberGenerator.hpp"
// #include "Engine/Renderer/RenderContext.hpp"
// 
// #include "Game/GlobalSystems.hpp"
// #include "Game/Game.hpp"
// 
// Debris::~Debris()
// {
// }
// 
// void Debris::Create()
// {
//     /*
// 
//     // Define the outer vertex based on n number of equal degree triangles
//     float degreesPerCorner = 360.f / ( float) m_TriangleCount;
//     float length = g_GameInstance->GetRng()->FloatInRange( m_InnerRadius,
//                                                    m_OuterRadius );
//     // First has angle of zero
//     Vec2 first = Vec2::MakeFromPolarDegrees( 0.f, length );
//     Vec2 prev = first;
// 
//     for ( int cornerIndex = 0; cornerIndex < m_TriangleCount; ++cornerIndex )
//     {
//         // First vertex is always (0, 0)
//         m_Visual.push_back( Vertex_PCU( Vec2::ZERO, m_DebrisColor, Vec2( .5f, .5f ) ) );
// 
//         float currentDegree = degreesPerCorner * (cornerIndex + 1);
//         length = g_GameInstance->GetRng()->FloatInRange( m_InnerRadius, m_OuterRadius );
// 
//         Vec2 current = Vec2::ZERO;
//         Vec2 uvMap = Vec2::ZERO;
//         if ( cornerIndex == m_TriangleCount - 1 )
//         {
//             current = Vec2::MakeFromPolarDegrees( currentDegree, length );
// 
//             uvMap = Vec2( RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, current.x ),
//                           RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, current.y ) );
//             m_Visual.push_back( Vertex_PCU( current, m_DebrisColor, uvMap ) );
// 
//             uvMap = Vec2( RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, first.x ),
//                           RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, first.y ) );
//             m_Visual.push_back( Vertex_PCU( first, m_DebrisColor, uvMap ) );
//         }
//         else
//         {
//             uvMap = Vec2( RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, prev.x ),
//                           RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, prev.y ) );
//             m_Visual.push_back( Vertex_PCU( prev, m_DebrisColor, uvMap ) );
// 
//             current = Vec2::MakeFromPolarDegrees( currentDegree, length );
// 
//             uvMap = Vec2( RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, current.x ),
//                           RangeMapFloat( 0.f, m_OuterRadius, .5f, 1.f, current.y ) );
//             m_Visual.push_back( Vertex_PCU( current, m_DebrisColor, uvMap ) );
//         }
// 
//         prev = current;
//     }
// 
//     */
// }
// 
// void Debris::Update( float deltaSeconds )
// {
//     if ( m_Age > m_LifeSpan )
//     {
//         Die();
//     }
// 
//     // Update the color of the debris
//     if ( m_FadeOut )
//     {
//         float alphaModifier = RangeMapFloat( 0.f, m_LifeSpan, .5f, 0.f, m_Age );
//         m_DebrisColor.SetAlphaAsPercent( alphaModifier );
//     }
// 
//     Entity2D::Update( deltaSeconds );
// }
// 
// void Debris::Render() const
// {
//     std::vector<Vertex_PCU> visualCopy( m_Visual );
//     ChangeVertexArray( visualCopy, m_DebrisColor );
// 
//     TransformVertexArray( visualCopy, m_AngleDegrees, m_Position );
// 
//     g_Renderer->BindTexture( nullptr );
//     g_Renderer->DrawVertexArray( visualCopy );
// }
// 
// void Debris::Die()
// {
//     m_IsDead = true;
//     m_IsGarbage = true;
// }
// 
// void Debris::Destroy()
// {
// }
// 
// Debris::Debris( Game* gameInstance,
//                 Map* mapInstance,
//                 const Vec2& startingPosition,
//                 const Vec2& initialVelocity,
//                 float angularVelocity,
//                 Vec2 scale,
//                 float lifeSpan,
//                 const Rgba8& color,
//                 bool fadeOut,
//                 int triangeCount,
//                 float innerRadius,
//                 float outerRadius )
//     :Entity2D( gameInstance, mapInstance, startingPosition, 0.f, scale )
//     , m_LifeSpan( lifeSpan )
//     , m_FadeOut( fadeOut )
//     , m_DebrisColor( color )
//     , m_InnerRadius( innerRadius )
//     , m_OuterRadius( outerRadius )
// {
//     m_Velocity = initialVelocity;
//     m_AngularVelocity = angularVelocity;
// 
//     m_TriangleCount = triangeCount;
// }
// 
// DebrisType DebrisType::UniformDebrisExplosion( Game* gameInstance,
//                                                Map* mapInstance,
//                                                const Vec2& centerPointSpawn,
//                                                float initialSpeed )
// {
//     DebrisType uniformDebris = DebrisType( gameInstance,
//                                            mapInstance,
//                                            centerPointSpawn );
//     uniformDebris.m_InitialSpeed = initialSpeed;
//     uniformDebris.m_LowerAngularDisplacement = 0.f;
//     uniformDebris.m_UpperAngularDisplacement = 360.f;
// 
//     return uniformDebris;
// }
// 
// DebrisType DebrisType::ShotgunWithVelocity( Game* gameInstance,
//                                             Map* mapInstance,
//                                             const Vec2& centerPointSpawn,
//                                             float initialSpeed,
//                                             const Vec2& direction,
//                                             float angularDistance )
// {
//     DebrisType shotgunWith = DebrisType( gameInstance,
//                                          mapInstance,
//                                          centerPointSpawn );
//     shotgunWith.m_InitialSpeed = initialSpeed;
//     shotgunWith.m_InitialVelocityDirection = direction.GetNormalized();
//     shotgunWith.m_LowerAngularDisplacement = -angularDistance;
//     shotgunWith.m_UpperAngularDisplacement = angularDistance;
// 
//     return shotgunWith;
// }
// 
// DebrisType::DebrisType( Game* gameInstance,
//                         Map* mapInstance,
//                         const Vec2& centerPointSpawn )
//     : m_GameInstance( gameInstance )
//     , m_MapInstance( mapInstance )
//     , m_CenterPointSpawn( centerPointSpawn )
// {
// }
// 
// DebrisType& DebrisType::SetCircularSpawnRadius( float circularRadius )
// {
//     m_CircleRadius = circularRadius;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetInitialSpeed( float initialSpeed )
// {
//     m_InitialSpeed = initialSpeed;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetSpeedRangeChange( float lowerRange, float upperRange )
// {
//     m_LowerInitialSpeedRange = lowerRange;
//     m_UpperInitialSpeedRange = upperRange;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetVelocityDirection( float initialDirection )
// {
//     m_InitialVelocityDirection = Vec2::MakeFromPolarDegrees( initialDirection );
//     return *this;
// }
// 
// DebrisType& DebrisType::SetInitialScale( const Vec2& initialScale )
// {
//     m_InitialScale = initialScale;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetScaleRangeChange( float lowerScaleRange, float upperScaleRange )
// {
//     m_LowerScaleRange = lowerScaleRange;
//     m_UpperScaleRange = upperScaleRange;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetVelocityDirectionRangeChange( float lowerAngularDisplacement, float upperAngularDisplacement )
// {
//     m_LowerAngularDisplacement = lowerAngularDisplacement;
//     m_UpperAngularDisplacement = upperAngularDisplacement;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetInitialAngularVelocity( float angularVelocity )
// {
//     m_InitialAngularVelocity = angularVelocity;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetAngularVelocityRangeChange( float lowerAngularVelocity, float upperAngularVelocity )
// {
//     m_LowerAngularVelocity = lowerAngularVelocity;
//     m_UpperAngularVelocity = upperAngularVelocity;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetLifeSpan( float lifeSpan )
// {
//     m_LifeSpan = lifeSpan;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetInitialColor( const Rgba8& color )
// {
//     m_InitialColor = color;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetFadeOut( bool fadeOut )
// {
//     m_FadeOut = fadeOut;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetTriangeCount( int triangleCount )
// {
//     m_TriangleCount = triangleCount;
//     return *this;
// }
// 
// DebrisType& DebrisType::SetTriangleRadiusRange( float innerRadius, float outerRadius )
// {
//     m_InnerRadius = innerRadius;
//     m_OuterRadius = outerRadius;
//     return *this;
// }
// 
// Debris* DebrisType::BuildDebris()
// {
//     RandomNumberGenerator* rng = m_GameInstance->GetRng();
//     Vec2 spawnPosition = m_CenterPointSpawn + Vec2::MakeFromPolarDegrees(
//         rng->FloatLessThan( 360.f ),
//         m_CircleRadius
//     );
// 
//     Vec2 spawnVelocity = m_InitialVelocityDirection.GetRotatedDegrees(
//         rng->FloatInRange( m_LowerAngularDisplacement, m_UpperAngularDisplacement ) );
//     spawnVelocity.SetLength( m_InitialSpeed + rng->FloatInRange( m_LowerInitialSpeedRange,
//                                                                  m_UpperInitialSpeedRange )
//     );
// 
//     float spawnAngularVelocity = m_InitialAngularVelocity + rng->FloatInRange( m_LowerAngularVelocity,
//                                                                                m_UpperAngularVelocity
//     );
// 
//     Vec2 spawnScale = m_InitialScale + Vec2( rng->FloatInRange( m_LowerScaleRange,
//                                                                 m_UpperScaleRange ),
//                                              rng->FloatInRange( m_LowerScaleRange,
//                                                                 m_UpperScaleRange)
//     );
// 
//     Debris * newDebris = new Debris( m_GameInstance,
//                                      m_MapInstance,
//                                      spawnPosition,
//                                      spawnVelocity,
//                                      spawnAngularVelocity,
//                                      spawnScale,
//                                      m_LifeSpan,
//                                      m_InitialColor,
//                                      m_FadeOut,
//                                      m_TriangleCount,
//                                      m_InnerRadius,
//                                      m_OuterRadius );
//     newDebris->Create();
// 
//     return newDebris;
// }
