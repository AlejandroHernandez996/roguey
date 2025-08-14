#pragma once

#include "CoreMinimal.h"
#include "rogueyStat.generated.h"

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyStat
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 CurrentLevel = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 BaseLevel = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 StatBoost = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 CurrentExperience = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 TotalExperience = 0; // The new total XP tracker
    
    static const int32 ExperienceFactor = 100;

    int32 GetExperienceRequiredForLevel(int32 Level) const
    {
        if (Level <= 0) return -1;
        if (Level == 1) return 0;
        
        return ExperienceFactor * (Level - 1) * Level / 2;
    }

    bool IncrementExperience(int32 GainedExperience)
    {
        CurrentExperience += GainedExperience;
        TotalExperience += GainedExperience; 
        
        const int32 XpForNextLevelCumulative = GetExperienceRequiredForLevel(CurrentLevel + 1);

        if (CurrentExperience >= XpForNextLevelCumulative)
        {
            BaseLevel++;
            CurrentLevel = BaseLevel;
            CurrentExperience -= XpForNextLevelCumulative;
            return true;
        }
        return false;
    }
};