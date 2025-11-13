#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SNMontageSetFactory.generated.h"

UCLASS()
class SCENARIONARRATIVEEDITOR_API USNMontageSetFactory : public UFactory
{
    GENERATED_BODY()

public:
    USNMontageSetFactory();

    TObjectPtr<UAnimSequenceBase> SourceObject;

    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context,
                                      FFeedbackContext* Warn) override;
};
