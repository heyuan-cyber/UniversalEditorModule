#pragma once

#include "CoreMinimal.h"
#include "SNSequencerFactory.generated.h"

UCLASS()
class SCENARIONARRATIVEEDITOR_API USNSequencerFactory : public UFactory
{
    GENERATED_BODY()

public:
    USNSequencerFactory();
    virtual ~USNSequencerFactory() override;

    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                      UObject* Context, FFeedbackContext* Warn) override;
};
