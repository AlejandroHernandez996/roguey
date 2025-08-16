#include "rogueyDamageCalculator.h"

#include "Characters/Player/rogueyCharacter.h"
#include "Characters/Player/Controller/rogueyPlayerController.h"
#include "Combat/CombatEvent.h"

void UrogueyDamageCalculator::CalculateCombat(int32 TickIndex, FCombatEvent CombatEvent)
{
    TWeakObjectPtr<ArogueyPawn> FromPawn = CombatEvent.FromActor;
    TWeakObjectPtr<ArogueyPawn> ToPawn = CombatEvent.ToActor;

    if (!FromPawn.Get() || !ToPawn.Get())
    {
        return;
    }

    if (TickIndex - FromPawn->LastAttackTickIndex > FromPawn->StatPage.StatPage[ErogueyStatType::ATTACK_SPEED].CurrentLevel)
    {
       int32 DamageDealt = 0; // Default damage is 0 (a miss)
       
       if (CanHit(FromPawn, ToPawn))
       {
           DamageDealt = CalculateDamage(FromPawn); // Only calculate damage if the attack hits
       }

        ToPawn->UpdateCurrentStat(ErogueyStatType::HEALTH, -DamageDealt);
       
        FromPawn->SetPawnState(EPawnState::ATTACKING, true);
        FromPawn->LastAttackTickIndex = TickIndex;
        int32 ExperienceAmount = DamageDealt * 4;
        bool bLeveledUp = FromPawn->StatPage.StatPage[ErogueyStatType::STRENGTH].IncrementExperience(ExperienceAmount);
        ArogueyCharacter* Character = Cast<ArogueyCharacter>(FromPawn);
        if (Character)
        {
            if (bLeveledUp)
            {
                Cast<ArogueyPlayerController>(Character)->OnChatMessage.Broadcast("Leveled Up!");
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
    
    // The attack hits if the attacker's roll is higher than the defender's roll
    return AttackerAttackRoll > DefenderDefenseRoll;
}

int32 UrogueyDamageCalculator::GetAttackRoll(TWeakObjectPtr<ArogueyPawn> FromPawn)
{
    // A simplified attack roll formula based on the attacker's attack level
    int32 AttackLevel = FromPawn->StatPage.StatPage[ErogueyStatType::ATTACK].CurrentLevel;
    int32 AttackRoll = FMath::RandRange(1, AttackLevel + 5); // Add a small base value to prevent easy misses at low levels
    
    return AttackRoll;
}

int32 UrogueyDamageCalculator::GetDefenceRoll(TWeakObjectPtr<ArogueyPawn> ToPawn)
{
    // A simplified defense roll formula based on the defender's defense level
    int32 DefenseLevel = ToPawn->StatPage.StatPage[ErogueyStatType::DEFENCE].CurrentLevel;
    int32 DefenseRoll = FMath::RandRange(1, DefenseLevel + 5); // Add a small base value to prevent easy misses at low levels
    
    return DefenseRoll;
}

int32 UrogueyDamageCalculator::CalculateDamage(TWeakObjectPtr<ArogueyPawn> FromPawn)
{
    if (!FromPawn.Get())
    {
        return 0;
    }
    
    int32 AttackerStrengthLevel = FromPawn->StatPage.StatPage[ErogueyStatType::STRENGTH].CurrentLevel;
    int32 MaxHit = GetDamageBonusFromStrength(AttackerStrengthLevel);
    
    return FMath::RandRange(1, MaxHit);
}

int32 UrogueyDamageCalculator::GetDamageBonusFromStrength(int32 StrengthLevel)
{
    return (StrengthLevel / 5) + 1;
}