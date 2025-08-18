#include "rogueyDamageCalculator.h"

#include "rogueyGameMode.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Characters/Player/Controller/rogueyPlayerController.h"
#include "Combat/CombatEvent.h"
#include "Inventory/rogueyInventoryManager.h"

void UrogueyDamageCalculator::CalculateCombat(int32 TickIndex, FCombatEvent CombatEvent)
{
    TWeakObjectPtr<ArogueyPawn> FromPawn = CombatEvent.FromActor;
    TWeakObjectPtr<ArogueyPawn> ToPawn = CombatEvent.ToActor;

    if (!FromPawn.Get() || !ToPawn.Get())
    {
        return;
    }

    if (TickIndex - FromPawn->LastAttackTickIndex > FromPawn->LastAttackCooldown)
    {
        int32 DamageDealt = 0; 
       
        if (CanHit(FromPawn, ToPawn))
        {
           DamageDealt = CalculateDamage(FromPawn);
        }

        ToPawn->UpdateCurrentStat(ErogueyStatType::HEALTH, -DamageDealt);
       
        FromPawn->SetPawnState(EPawnState::ATTACKING, true);
        FromPawn->LastAttackTickIndex = TickIndex;

        int32 AttackCooldown = FromPawn->DefaultAttackCooldown;
        if (ArogueyCharacter* PlayerCharacter = Cast<ArogueyCharacter>(FromPawn))
        {
            ArogueyGameMode* RogueyGameMode = Cast<ArogueyGameMode>(PlayerCharacter->GetWorld()->GetAuthGameMode());
            AttackCooldown = RogueyGameMode->InventoryManager->GetTotalBonusByStat(EItemStatType::ATTACK_SPEED);
        }
        FromPawn->LastAttackCooldown = AttackCooldown;
        
        int32 ExperienceAmount = DamageDealt * 4;
        bool bLeveledUp = FromPawn->StatPage.StatPage[ErogueyStatType::STRENGTH].IncrementExperience(ExperienceAmount);
        ArogueyCharacter* Character = Cast<ArogueyCharacter>(FromPawn);
        if (Character)
        {
            if (bLeveledUp)
            {
               Cast<ArogueyPlayerController>(Character->GetController())->OnChatMessage.Broadcast("Leveled Up!");
            }
            if (DamageDealt > 0)
            {
                Character->OnExperienceDrop.Broadcast(ExperienceAmount, ErogueyStatType::STRENGTH);
            }
        }
       
       if (DamageDealt > 0)
       {
           UE_LOG(LogTemp, Warning, TEXT("%s attacked %s for %d damage."), *FromPawn->GetName(), *ToPawn->GetName(), DamageDealt);
       }
       else
       {
           UE_LOG(LogTemp, Warning, TEXT("%s attacked %s but missed."), *FromPawn->GetName(), *ToPawn->GetName());
       }
    }
}

bool UrogueyDamageCalculator::CanHit(TWeakObjectPtr<ArogueyPawn> FromPawn, TWeakObjectPtr<ArogueyPawn> ToPawn)
{
    int32 AttackerAttackRoll = GetAttackRoll(FromPawn.Get());
    int32 DefenderDefenseRoll = GetDefenceRoll(ToPawn.Get());
    
    return AttackerAttackRoll > DefenderDefenseRoll;
}

int32 UrogueyDamageCalculator::GetAttackRoll(TWeakObjectPtr<ArogueyPawn> FromPawn)
{
    int32 AttackLevel = FromPawn->StatPage.StatPage[ErogueyStatType::ATTACK].CurrentLevel;
    int32 AttackRoll = FMath::RandRange(1, AttackLevel + 5); 
    
    return AttackRoll;
}

int32 UrogueyDamageCalculator::GetDefenceRoll(TWeakObjectPtr<ArogueyPawn> ToPawn)
{
    int32 DefenseLevel = ToPawn->StatPage.StatPage[ErogueyStatType::DEFENCE].CurrentLevel;
    int32 DefenseRoll = FMath::RandRange(1, DefenseLevel + 5);
    
    return DefenseRoll;
}

int32 UrogueyDamageCalculator::CalculateDamage(TWeakObjectPtr<ArogueyPawn> FromPawn)
{
    if (!FromPawn.Get())
    {
        return 0;
    }
    
    int32 AttackerStrengthLevel = FromPawn->StatPage.StatPage[ErogueyStatType::STRENGTH].CurrentLevel;
    int32 EquippedItemStrengthBonus = 0;
    if (ArogueyCharacter* PlayerCharacter = Cast<ArogueyCharacter>(FromPawn))
    {
        ArogueyGameMode* RogueyGameMode = Cast<ArogueyGameMode>(PlayerCharacter->GetWorld()->GetAuthGameMode());
        EquippedItemStrengthBonus = RogueyGameMode->InventoryManager->GetTotalBonusByStat(EItemStatType::STRENGTH);
    }
    AttackerStrengthLevel  += EquippedItemStrengthBonus;
    int32 MaxHit = GetDamageBonusFromStrength(EquippedItemStrengthBonus);
    
    return FMath::RandRange(1, MaxHit);
}

int32 UrogueyDamageCalculator::GetDamageBonusFromStrength(int32 StrengthLevel)
{
    return (StrengthLevel / 5) + 1;
}