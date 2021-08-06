#include "SB_UtilityLibrary.h"
#include "Engine/Engine.h"


SB_UtilityLibrary::SB_UtilityLibrary()
{

}

void SB_UtilityLibrary::PrintStringToScreen(const FString& PrefixString, const FString& InputString, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + InputString;

	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
}

void SB_UtilityLibrary::PrintFloatToScreen(const FString& PrefixString, float InputFloat, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + FString::SanitizeFloat(InputFloat, 4);

	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
}

void SB_UtilityLibrary::PrintVectorToScreen(const FString& PrefixString, FVector InputVector, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString
		+ "X : " + FString::SanitizeFloat(InputVector.X, 4)
		+ "Y : " + FString::SanitizeFloat(InputVector.Y, 4)
		+ "Z : " + FString::SanitizeFloat(InputVector.Z, 4);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
	}
}

void SB_UtilityLibrary::PrintBoolToScreen(bool bBoolToPrint, FString PrefixString, float TimeToDisplay)
{
	FString StringToPrint;
	if (bBoolToPrint)
		StringToPrint = "true";
	else
		StringToPrint = "false";

	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, PrefixString + FString::Printf(TEXT(" : %s"), *StringToPrint));
}

void SB_UtilityLibrary::PrintRotatorToScreen(FRotator RotatorToPrint, FString PrefixString, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + " / Pitch : " + FString::SanitizeFloat(RotatorToPrint.Pitch) + " / Yaw : " + FString::SanitizeFloat(RotatorToPrint.Yaw) + " / Roll : " + FString::SanitizeFloat(RotatorToPrint.Roll);
	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, FColor::Magenta, StringToPrint);
}

AActor* SB_UtilityLibrary::GetClosestActorFromLocation(TArray<AActor*> ActorsToCheck, FVector TargetLocation)
{
	return nullptr;
}

/*template<typename TEnum>
void VCUtilityLibrary::PrintEnumToScreen(const FString& Name, TEnum Value)
{
	FString StringToPrint = this->GetEnumAsString(Name, Value);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, StringToPrint);
}*/

/*VCUtilityLibrary::~VCUtilityLibrary()
{

}*/
