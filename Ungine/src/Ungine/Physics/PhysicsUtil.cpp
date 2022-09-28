#include "Upch.h"
#include "PhysicsUtil.h"

#include "Ungine/Script/ScriptEngine.h"

namespace U
{
	physx::PxTransform ToPhysXTransform(const glm::mat4& matrix)
	{
		physx::PxQuat r = ToPhysXQuat(glm::normalize(glm::toQuat(matrix)));
		physx::PxVec3 p = ToPhysXVector(glm::vec3(matrix[3]));
		return physx::PxTransform(p, r);
	}

	physx::PxMat44 ToPhysXMatrix(const glm::mat4& matrix)
	{
		return *(physx::PxMat44*)&matrix;
	}

	physx::PxVec3 ToPhysXVector(const glm::vec3& vector)
	{
		return physx::PxVec3(vector.x, vector.y, vector.z);
	}

	physx::PxVec4 ToPhysXVector(const glm::vec4& vector)
	{
		return physx::PxVec4(vector.x, vector.y, vector.z, vector.w);
	}

	physx::PxQuat ToPhysXQuat(const glm::quat& quat)
	{
		return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
	}

	glm::mat4 FromPhysXTransform(const physx::PxTransform& transform)
	{
		glm::quat rotation = FromPhysXQuat(transform.q);
		glm::vec3 position = FromPhysXVector(transform.p);
		return glm::translate(glm::mat4(1.0F), position) * glm::toMat4(rotation);
	}

	glm::mat4 FromPhysXMatrix(const physx::PxMat44& matrix)
	{
		return *(glm::mat4*)&matrix;
	}

	glm::vec3 FromPhysXVector(const physx::PxVec3& vector)
	{
		return glm::vec3(vector.x, vector.y, vector.z);
	}

	glm::vec4 FromPhysXVector(const physx::PxVec4& vector)
	{
		return glm::vec4(vector.x, vector.y, vector.z, vector.w);
	}

	glm::quat FromPhysXQuat(const physx::PxQuat& quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	}

	physx::PxFilterFlags UngineFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		}

		return physx::PxFilterFlag::eDEFAULT;
	}

	void ContactListener::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		PX_UNUSED(constraints);
		PX_UNUSED(count);
	}

	void ContactListener::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		PX_UNUSED(actors);
		PX_UNUSED(count);
	}

	void ContactListener::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		PX_UNUSED(actors);
		PX_UNUSED(count);
	}

	void ContactListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		Entity& a = *(Entity*)pairHeader.actors[0]->userData;
		Entity& b = *(Entity*)pairHeader.actors[1]->userData;

		if (pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
		{
			if (ScriptEngine::IsEntityModuleValid(a)) ScriptEngine::OnCollisionBegin(a);
			if (ScriptEngine::IsEntityModuleValid(b)) ScriptEngine::OnCollisionBegin(b);
		}
		else if (pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
		{
			if (ScriptEngine::IsEntityModuleValid(a)) ScriptEngine::OnCollisionEnd(a);
			if (ScriptEngine::IsEntityModuleValid(b)) ScriptEngine::OnCollisionEnd(b);
		}
	}

	void ContactListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		Entity& a = *(Entity*)pairs->triggerActor->userData;
		Entity& b = *(Entity*)pairs->otherActor->userData;

		if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (ScriptEngine::IsEntityModuleValid(a)) ScriptEngine::OnTriggerBegin(a);
			if (ScriptEngine::IsEntityModuleValid(b)) ScriptEngine::OnTriggerBegin(b);
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (ScriptEngine::IsEntityModuleValid(a)) ScriptEngine::OnTriggerEnd(a);
			if (ScriptEngine::IsEntityModuleValid(b)) ScriptEngine::OnTriggerEnd(b);
		}
	}

	void ContactListener::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		PX_UNUSED(bodyBuffer);
		PX_UNUSED(poseBuffer);
		PX_UNUSED(count);
	}


}