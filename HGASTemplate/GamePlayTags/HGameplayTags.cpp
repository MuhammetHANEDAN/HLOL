
#include "HGameplayTags.h"
#include "GameplayTagsManager.h"

FHGamePlayTags FHGamePlayTags::GameplayTags;

void FHGamePlayTags::InitializeNativeGameplayTags()
{
	/*
	 * Map of Damage Types to Armors
	 */
	
	GameplayTags.DamageTypesToArmors.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_PhysicalArmor);
	GameplayTags.DamageTypesToArmors.Add(GameplayTags.Damage_Magical, GameplayTags.Attributes_MagicalArmor);

	/*
	 * Map of Penetration Types to Armors
	 */

	GameplayTags.PenetrationTypesToArmors.Add(GameplayTags.Attributes_PhysicalArmorPenetration,GameplayTags.Attributes_PhysicalArmor);
	GameplayTags.PenetrationTypesToArmors.Add(GameplayTags.Attributes_MagicalArmorPenetration,GameplayTags.Attributes_MagicalArmor);
	
	
	/**
	 * Heroes
	 */

	GameplayTags.Heroes_Grux= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Heroes.Grux"),
		FString("Hero Grux")
		);

	GameplayTags.Heroes_Sparrow= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Heroes.Sparrow"),
		FString("Hero Sparrow")
		);

	GameplayTags.Heroes_Narbash= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Heroes.Narbash"),
		FString("Hero Narbash")
		);

	GameplayTags.Heroes_Gideon= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Heroes.Gideon"),
		FString("Hero Gideon")
		);

	/**
	 * Events
	 */

	GameplayTags.Event_Anim_True= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Anim.True"),
		FString("Event Anim True")
		);

	GameplayTags.Event_Anim_False= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Anim.False"),
		FString("Event Anim False")
		);

	GameplayTags.Event_Attacked= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attacked"),
		FString("Event Attacked")
		);
	
	/**
	 * Attributes
	 */

	GameplayTags.Attributes_MaxHealth= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxHealth"),
		FString("Max Health")
		);

	GameplayTags.Attributes_Health= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Health"),
		FString("Health")
		);

	GameplayTags.Attributes_HealthRegen= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.HealthRegen"),
		FString("Health Regen")
		);

	GameplayTags.Attributes_MaxMana= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxMana"),
		FString("MaxMana")
		);
	
	GameplayTags.Attributes_Mana= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Mana"),
		FString("Mana")
		);

	GameplayTags.Attributes_ManaRegen= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.ManaRegen"),
		FString("ManaRegen")
		);

	GameplayTags.Attributes_AttackDamage= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AttackDamage"),
		FString("AttackDamage")
		);

	GameplayTags.Attributes_AttackSpeed= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AttackSpeed"),
		FString("AttackSpeed")
		);
	
	GameplayTags.Attributes_AbilityPower= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AbilityPower"),
		FString("AbilityPower")
		);

	GameplayTags.Attributes_AttackRange= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AttackRange"),
		FString("AttackRange")
		);

	GameplayTags.Attributes_PhysicalArmor= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.PhysicalArmor"),
		FString("PhysicalArmor")
		);

	GameplayTags.Attributes_MagicalArmor= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MagicalArmor"),
		FString("MagicalArmor")
		);

	GameplayTags.Attributes_CriticalHitChance= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.CriticalHitChance"),
		FString("CriticalHitChance")
		);

	GameplayTags.Attributes_CriticalHitDamageCoefficient= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.CriticalHitDamageCoefficient"),
		FString("CriticalHitDamageCoefficient")
		);

	GameplayTags.Attributes_LifeSteal= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.LifeSteal"),
		FString("LifeSteal")
		);

	GameplayTags.Attributes_BashChance= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.BashChance"),
		FString("BashChance")
		);

	GameplayTags.Attributes_NormalAttackSlow= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.NormalAttackSlow"),
		FString("NormalAttackSlow")
		);

	GameplayTags.Attributes_CooldownReduction= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.CooldownReduction"),
		FString("CooldownReduction")
		);
	
	GameplayTags.Attributes_PhysicalArmorPenetration= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.PhysicalArmorPenetration"),
		FString("PhysicalPenetration")
		);

	GameplayTags.Attributes_MagicalArmorPenetration= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MagicalArmorPenetration"),
		FString("MagicPenetration")
		);

	GameplayTags.Attributes_MoveSpeed= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MoveSpeed"),
		FString("MoveSpeed")
		);
	

	/*
	 * Input Tags
	 */
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
		);
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
		);
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Q"),
		FString("Input Tag for Q key")
		);
	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.W"),
		FString("Input Tag for W key")
		);
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("Input Tag for E key")
		);
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.R"),
		FString("Input Tag for R key")
		);

	GameplayTags.InputTag_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive"),
		FString("Input Tag Passive Ability ")
		);

	GameplayTags.NormalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("NormalAttack"),
		FString("NormalAttack ")
		);

	GameplayTags.InputTag_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Block"),
		FString("InputTagBlock")
		);

	GameplayTags.InputTag_Recall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Recall"),
		FString("InputTagRecall")
		);

	/**
	 * Status
	 */

	GameplayTags.Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Locked"),
		FString("Status Locked")
		);

	GameplayTags.Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Unlocked"),
		FString("Status Unlocked")
		);

	
	/**
	 * Ability Events
	 */

	GameplayTags.AbilityEvent_CancelNormalAttackAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityEvent.CancelNormalAttackAbility"),
		FString("Cancel NormalAttack Ability")
		);

	/**
	 * Damage Types
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage"),
	FString("Damage")
	);
	
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Physical"),
	FString("Physical Damage Type")
	);

	GameplayTags.Damage_Magical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Magical"),
	FString("Magical Damage Type")
	);

	GameplayTags.Damage_True= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.True"),
	FString("True Damage Type")
	);

	GameplayTags.GEffect_Skill= UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GEffect.Skill"),
	FString("GEffect_Skill")
	);

	// Set By Caller Tags

	GameplayTags.Set_HP = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Set.HP"),
		FString("Set HP")
		);

	GameplayTags.Set_MP = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Set.MP"),
		FString("Set MP")
		);
	

	/**
	 * States
	 */

	GameplayTags.State_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Dead"),
		FString("Dead")
		);

	GameplayTags.State_Attacking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Attacking"),
		FString("Attacking")
		);

	GameplayTags.State_Casting = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Casting"),
		FString("Casting")
		);

	GameplayTags.State_DamageImmunity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.DamageImmunity"),
		FString("DamageImmunity")
		);

	GameplayTags.State_Alive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("State.Alive"),
		FString("Alive")
		);
	
	
	/*
	 * Debuffs
	 */

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff for Stun")
		);

	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff for Fire damage")
		);

	GameplayTags.Debuff_Slow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Slow"),
		FString("Debuff for Slow")
		);
	
	GameplayTags.Debuff_Silence = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Debuff.Silence"),
			FString("Debuff for Silence")
			);

	GameplayTags.Debuff_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Debuff.Knockback"),
			FString("Debuff for Knockback")
			);
	
	
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
		);
	
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
		);
	
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
		);

	/*
	 * Meta Attributes
	 */

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
		);

	GameplayTags.Attributes_Meta_IncomingDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingDamage"),
		FString("Incoming Damage Meta Attribute")
		);

	
	/**
	 * Effects
	 */

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting")
		);

	/**
	 * Ability Types
	 */

	GameplayTags.AbilityTypes_Action = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityTypes.Action"),
		FString("AbilityTypes Action")
		);

	/*
	 * Abilities
	 */

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability - like the nullptr for Ability Tags")
		);

	GameplayTags.Abilities_Die = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Die"),
		FString("Die Ability")
		);

	GameplayTags.Abilities_Restart = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Restart"),
		FString("Restart Ability")
		);

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
		);

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag")
		);

	/*
	 * Offensive Spells
	 */

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag")
		);

	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"),
		FString("FireBlast Ability Tag")
		);

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag")
		);

	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
		FString("Arcane Shards Ability Tag")
		);


	

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability")
		);
	
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status")
		);
	
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status")
		);
	
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status")
		);
	
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status")
		);
	
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None")
		);
	
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Type Offensive")
		);
	
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Type Passive")
		);

	
	/*
	 * Passive Spells
	 */
	
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.LifeSiphon"),
			FString("Life Siphon")
			);
	
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.ManaSiphon"),
			FString("Mana Siphon")
			);
	
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.HaloOfProtection"),
			FString("Halo Of Protection")
			);

	/*
	* Cooldown
	*/

	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag")
		);

	GameplayTags.Cooldown_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Q"),
		FString("Q Cooldown Tag")
		);

	GameplayTags.Cooldown_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.W"),
		FString("W Cooldown Tag")
		);

	GameplayTags.Cooldown_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.E"),
		FString("E Cooldown Tag")
		);

	GameplayTags.Cooldown_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.R"),
		FString("R Cooldown Tag")
		);

	/**
	 * Items
	 */

	GameplayTags.Item_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.1"),
		FString("Item_1")
		);

	GameplayTags.Item_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.2"),
		FString("Item_2")
		);

	GameplayTags.Item_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.3"),
		FString("Item_3")
		);

	GameplayTags.Item_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.4"),
		FString("Item_4")
		);

	GameplayTags.Item_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.5"),
		FString("Item_5")
		);

	GameplayTags.Item_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.6"),
		FString("Item_6")
		);

	GameplayTags.Item_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.7"),
		FString("Item_7")
		);

	GameplayTags.Item_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.8"),
		FString("Item_8")
		);

	GameplayTags.Item_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.9"),
		FString("Item_9")
		);

	/**
	 * Item Abilities
	 */

	GameplayTags.Item_Mjolnir = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Item.Mjolnir"),
		FString("Item_Mjolnir")
		);

	GameplayTags.Cooldown_Mjolnir = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Mjolnir"),
		FString("Cooldown_Mjolnir")
		);

	/*
	 * GameplayCues
	 */

	GameplayTags.GameplayCue_Mjolnir = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Mjolnir"),
		FString("Mjolnir GameplayCue Tag")
		);

	GameplayTags.GameplayCue_ReturnBase = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.ReturnBase"),
		FString("ReturnBase GameplayCue Tag")
		);

	GameplayTags.GameplayCue_RedBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.RedBuff"),
		FString("RedBuff GameplayCue Tag")
		);

	GameplayTags.GameplayCue_BlueBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.BlueBuff"),
		FString("BlueBuff GameplayCue Tag")
		);

	GameplayTags.GameplayCue_PBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.PBuff"),
		FString("PBuff GameplayCue Tag")
		);

	GameplayTags.GameplayCue_QBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.QBuff"),
		FString("QBuff GameplayCue Tag")
		);

	GameplayTags.GameplayCue_WBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.WBuff"),
		FString("WBuff GameplayCue Tag")
		);

	GameplayTags.GameplayCue_EBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.EBuff"),
		FString("EBuff GameplayCue Tag")
		);

	GameplayTags.GameplayCue_RBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.RBuff"),
		FString("RBuff GameplayCue Tag")
		);

	/**
	 * Effect Tags
	 */

	GameplayTags.EffectTag_RedBuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EffectTag.RedBuff"),
		FString("Effect Tag RedBuff")
		);
	
	GameplayTags.EffectTag_BlueBuff= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EffectTag.BlueBuff"),
		FString("Effect Tag BlueBuff")
		);

	GameplayTags.EffectTag_RemoveOnDeath= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EffectTag.RemoveOnDeath"),
		FString("Effect Tag RemoveOnDeath")
		);
	

	/*
	 * Combat Sockets
	 */

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
		);

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand")
		);

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand")
		);

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail")
		);

	/*
	 * Montage Tags
	 */

	GameplayTags.Montage_Rotation = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Rotation"),
		FString("Montage Rotation")
		);

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack 1")
		);

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack 2")
		);

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack 3")
		);

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack 4")
		);

	/*
	 * Player Tags
	 */
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Block tracing under the cursor")
		);
	
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input Held callback for input")
		);
	
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Block Input Pressed callback for input")
		);
	
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input Released callback for input")
		);

	
}