#pragma once

#include "GameplayTagContainer.h"

/**
 * Singleton containing Native Gameplay Tags
 */

struct FHGamePlayTags
{
	
public:
	
	static FHGamePlayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
	/**
	 * Heroes
	 */

	FGameplayTag Heroes_Grux;
	FGameplayTag Heroes_Sparrow;
	FGameplayTag Heroes_Narbash;
	FGameplayTag Heroes_Gideon;

	/**
	 * Events
	 */

	FGameplayTag Event_Anim_True;
	FGameplayTag Event_Anim_False;

	FGameplayTag Event_Attacked;

	/**
	 * Attributes
	 */

	TMap<FGameplayTag, FGameplayTag> DamageTypesToArmors;
	TMap<FGameplayTag, FGameplayTag> PenetrationTypesToArmors;
	
	
	// Sağlık ve Yenileme
	FGameplayTag Attributes_MaxHealth;               // Maksimum can
	FGameplayTag Attributes_HealthRegen;             // Can yenileme

	// Mana/Enerji ve Yenileme
	FGameplayTag Attributes_MaxMana;                 // Maksimum mana/enerji
	FGameplayTag Attributes_ManaRegen;               // Mana/enerji yenileme

	// Hasar ve Yetenek İlgili Attributeler
	FGameplayTag Attributes_AttackDamage;            // Fiziksel saldırı hasarı
	FGameplayTag Attributes_AbilityPower;            // Büyü gücü (ability power)
	FGameplayTag Attributes_AttackSpeed;             // Saldırı hızı
	FGameplayTag Attributes_AttackRange;             // Saldırı menzili

	// Savunma
	FGameplayTag Attributes_PhysicalArmor;                   // Fiziksel savunma (zırh)
	FGameplayTag Attributes_MagicalArmor;             // Büyü direnci

	// Kritik Vuruşlar
	FGameplayTag Attributes_CriticalHitChance;       // Kritik vuruş şansı
	FGameplayTag Attributes_CriticalHitDamageCoefficient;       // Kritik vuruş hasarı çarpanı

	// Can Yenileme ve Vamp Efektleri
	FGameplayTag Attributes_LifeSteal;               // Can çalma (fiziksel saldırıdan)
	FGameplayTag Attributes_BashChance;
	FGameplayTag Attributes_NormalAttackSlow;

	// Yetenek Soğuma Süresi ve Penetrasyonlar
	FGameplayTag Attributes_CooldownReduction;       // Yetenek bekleme süreleri kısaltma
	FGameplayTag Attributes_PhysicalArmorPenetration;     // Fiziksel zırh delme
	FGameplayTag Attributes_MagicalArmorPenetration;        // Büyü direnci delme

	//Vital
	//MaxHealth ve MaxManaya bağımlılar
	FGameplayTag Attributes_Health;           // Anlık can
	FGameplayTag Attributes_Mana;             // Anlık mana

	// Move Speed
	FGameplayTag Attributes_MoveSpeed;        // Hız

	// Input
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	FGameplayTag InputTag_Passive;
	FGameplayTag InputTag_Block;
	FGameplayTag InputTag_Recall;

	FGameplayTag NormalAttack;

	// Status
	FGameplayTag Status_Locked;
	FGameplayTag Status_Unlocked;

	// Ability Events
	FGameplayTag AbilityEvent_CancelNormalAttackAbility;
	
	// Damage Types
	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magical;
	FGameplayTag Damage_True;

	FGameplayTag GEffect_Skill; // Uygulanan effectin bir skill aracılığı ile uygulandığı anlamı taşır
	

	// Set By Caller Tags
	FGameplayTag Set_HP;
	FGameplayTag Set_MP;

	// Meta
	FGameplayTag Attributes_Meta_IncomingXP;
	FGameplayTag Attributes_Meta_IncomingDamage;

	// States
	FGameplayTag State_Dead;
	FGameplayTag State_Attacking;
	FGameplayTag State_Casting;
	FGameplayTag State_DamageImmunity;
	FGameplayTag State_Alive;
	
	

	//Debufs
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Slow;
	FGameplayTag Debuff_Silence;
	FGameplayTag Debuff_Knockback;
	
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	//Abilities
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Die;
	FGameplayTag Abilities_Restart;

	// İtems
	FGameplayTag Item_1;
	FGameplayTag Item_2;
	FGameplayTag Item_3;
	FGameplayTag Item_4;
	FGameplayTag Item_5;
	FGameplayTag Item_6;
	FGameplayTag Item_7;
	FGameplayTag Item_8;
	FGameplayTag Item_9;

	// Item Abilities
	FGameplayTag Item_Mjolnir;
	FGameplayTag Cooldown_Mjolnir;

	// Gameplay Cues
	FGameplayTag GameplayCue_Mjolnir;
	FGameplayTag GameplayCue_ReturnBase;
	FGameplayTag GameplayCue_RedBuff;
	FGameplayTag GameplayCue_BlueBuff;
	
	FGameplayTag GameplayCue_PBuff;
	FGameplayTag GameplayCue_QBuff;
	FGameplayTag GameplayCue_WBuff;
	FGameplayTag GameplayCue_EBuff;
	FGameplayTag GameplayCue_RBuff;
	

	// EffectTags
	FGameplayTag EffectTag_RedBuff;
	FGameplayTag EffectTag_BlueBuff;
	FGameplayTag EffectTag_RemoveOnDeath;


	
	/** Etkinleşince bir aksiyona giren abilityler için kullanılacak*/
	FGameplayTag AbilityTypes_Action;
	

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;	
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Arcane_ArcaneShards;
	
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	
	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag Cooldown_Q;
	FGameplayTag Cooldown_W;
	FGameplayTag Cooldown_E;
	FGameplayTag Cooldown_R;
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	// Montage Tags
	FGameplayTag Montage_Rotation;
	
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	FGameplayTag Effects_HitReact; // Tag granted when Hit Reacting

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

	



private:

	static FHGamePlayTags GameplayTags;
	
	
};