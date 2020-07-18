#pragma once

#include "Engine/Core/Math/Primatives/AABB2.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Utils/XmlUtils.hpp"

class EntityDefinition
{
public:
    EntityDefinition( const EntityDefinition& copy ) = default;

    //-------------------------------------------------------------------------
    // Entity Definition Accessors
    const std::string GetEntityDefName() const { return m_EntityDefinitionName; }
    float GetPhysicsRadius() const { return m_PhysicsRadius; }
    const AABB2 GetVisualBounds() const { return m_VisualBounds; }
    int GetMaxHealth() const { return m_MaxHealth; }
    int GetStartingHealth() const { return m_StartingHealth; }
    bool GetCanWalk() const { return m_CanWalk; }
    bool GetCanFly() const { return m_CanFly; }
    bool GetCanSwim() const { return m_CanSwim; }

protected:
    std::string m_EntityDefinitionName = "NO_DEFINITION_NAME";

    float m_PhysicsRadius = 1.f;
    AABB2 m_VisualBounds = AABB2::UNIT_BOX;

    int m_MaxHealth = 1;
    int m_StartingHealth = 1;

    bool m_CanWalk = false;
    bool m_CanFly = false;
    bool m_CanSwim = false;

    EntityDefinition( const XmlElement& element );

    void ParseSizeElement( const XmlElement* sizeElement );
    void ParseWalkElement( const XmlElement* walkElement );
    void ParseHealthElement( const XmlElement* healthElement );
};