#include "Attributes/GMCAttributes.h"

#include "GMCAbilityComponent.h"
#include "GMCAttributes.h"

void FAttribute::PostReplicatedAdd(const FGMCUnboundAttributeSet& InArraySerializer)
{
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->BroadcastAttributeChangeBySerializedItem(Tag, Value);
	}
}

void FAttribute::PostReplicatedChange(const struct FGMCUnboundAttributeSet& InArraySerializer)
{
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->BroadcastAttributeChangeBySerializedItem(Tag, Value);
	}
}
