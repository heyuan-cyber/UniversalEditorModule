#include "ScenarioNarrativeSequencer/SNSequencerFactory.h"

#include "Sequencer/SNSequencer.h"

USNSequencerFactory::USNSequencerFactory()
{
    SupportedClass = USNSequencer::StaticClass();
    bCreateNew = true;
}

USNSequencerFactory::~USNSequencerFactory()
{
}

UObject* USNSequencerFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                   UObject* Context, FFeedbackContext* Warn)
{
    USNSequencer* Object = NewObject<USNSequencer>(InParent, InClass, InName, Flags, Context);

    return Object;
}
