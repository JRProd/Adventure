#pragma once

class Map;
class Entity2D;

class World
{
public:
    World() {}
    ~World() {}

    void Create();
    void Update( float deltaSeconds );
    void Render() const;
    void DebugRender() const;
    void Destory();

    const Entity2D& GetPlayer() const;
    const Map& GetCurrentMap() const { return *m_CurrentMap; }

private:
    Map* m_CurrentMap = nullptr;

    void RegenerateCurrentMap();
};