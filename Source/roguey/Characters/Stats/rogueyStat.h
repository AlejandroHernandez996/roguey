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

    int64 CurrentExperience = 0;
    int64 TotalExperience = 0; 
    
    static const int32 MaxLevel = 126;

private:
    static int64 ExperienceTable[MaxLevel + 1];
    static bool bXPTableInitialized;

public:
    FrogueyStat()
    {
        InitializeXPTable();
    }

    static void InitializeXPTable()
    {
        if (bXPTableInitialized) return;

        ExperienceTable[1] = 0;
        for (int32 L = 2; L <= MaxLevel; ++L)
        {
            double diff = (L - 1) + 300.0 * FMath::Pow(2.0, (L - 1) / 7.0);
            ExperienceTable[L] = ExperienceTable[L - 1] + static_cast<int64>(diff / 4.0);
        }

        bXPTableInitialized = true;
    }

    int64 GetExperienceRequiredForLevel64(int32 Level) const
    {
        if (Level <= 1) return 0;
        if (Level > MaxLevel) Level = MaxLevel;
        return ExperienceTable[Level];
    }

    bool IncrementExperience(int64 GainedExperience)
    {
        CurrentExperience += GainedExperience;
        TotalExperience += GainedExperience;

        bool bLeveledUp = false;

        while (CurrentLevel < MaxLevel)
        {
            int64 XpForNextLevel = GetExperienceRequiredForLevel64(CurrentLevel + 1) -
                                   GetExperienceRequiredForLevel64(CurrentLevel);

            if (CurrentExperience >= XpForNextLevel)
            {
                CurrentExperience -= XpForNextLevel;
                BaseLevel++;
                CurrentLevel = BaseLevel;
                bLeveledUp = true;
            }
            else
            {
                break;
            }
        }

        return bLeveledUp;
    }
};

int64 FrogueyStat::ExperienceTable[FrogueyStat::MaxLevel + 1] = {};
bool FrogueyStat::bXPTableInitialized = false;
