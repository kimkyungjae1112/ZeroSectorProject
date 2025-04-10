#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroDialogueOptionDataTable.generated.h"

USTRUCT(BlueprintType)
struct FZeroDialogueOptionDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName RowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OptionDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float Reliability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bIsInterview;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TSoftObjectPtr<UDataTable> DataTable;
};