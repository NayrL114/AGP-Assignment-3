// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AGP/Pickups/WeaponPickup.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWeaponPickup() {}
// Cross Module References
	AGP_API UClass* Z_Construct_UClass_APickupBase();
	AGP_API UClass* Z_Construct_UClass_AWeaponPickup();
	AGP_API UClass* Z_Construct_UClass_AWeaponPickup_NoRegister();
	UPackage* Z_Construct_UPackage__Script_AGP();
// End Cross Module References
	void AWeaponPickup::StaticRegisterNativesAWeaponPickup()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AWeaponPickup);
	UClass* Z_Construct_UClass_AWeaponPickup_NoRegister()
	{
		return AWeaponPickup::StaticClass();
	}
	struct Z_Construct_UClass_AWeaponPickup_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AWeaponPickup_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APickupBase,
		(UObject* (*)())Z_Construct_UPackage__Script_AGP,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWeaponPickup_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Pickups/WeaponPickup.h" },
		{ "ModuleRelativePath", "Pickups/WeaponPickup.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AWeaponPickup_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AWeaponPickup>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AWeaponPickup_Statics::ClassParams = {
		&AWeaponPickup::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AWeaponPickup_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AWeaponPickup_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AWeaponPickup()
	{
		if (!Z_Registration_Info_UClass_AWeaponPickup.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AWeaponPickup.OuterSingleton, Z_Construct_UClass_AWeaponPickup_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AWeaponPickup.OuterSingleton;
	}
	template<> AGP_API UClass* StaticClass<AWeaponPickup>()
	{
		return AWeaponPickup::StaticClass();
	}
	AWeaponPickup::AWeaponPickup() {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AWeaponPickup);
	AWeaponPickup::~AWeaponPickup() {}
	struct Z_CompiledInDeferFile_FID_Ryan_Study_University_2023_Spring_41024_AGP_Assignment_3_Lab7StartingPoint_AGP_Source_AGP_Pickups_WeaponPickup_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Ryan_Study_University_2023_Spring_41024_AGP_Assignment_3_Lab7StartingPoint_AGP_Source_AGP_Pickups_WeaponPickup_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AWeaponPickup, AWeaponPickup::StaticClass, TEXT("AWeaponPickup"), &Z_Registration_Info_UClass_AWeaponPickup, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AWeaponPickup), 2321290575U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Ryan_Study_University_2023_Spring_41024_AGP_Assignment_3_Lab7StartingPoint_AGP_Source_AGP_Pickups_WeaponPickup_h_3232690351(TEXT("/Script/AGP"),
		Z_CompiledInDeferFile_FID_Ryan_Study_University_2023_Spring_41024_AGP_Assignment_3_Lab7StartingPoint_AGP_Source_AGP_Pickups_WeaponPickup_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Ryan_Study_University_2023_Spring_41024_AGP_Assignment_3_Lab7StartingPoint_AGP_Source_AGP_Pickups_WeaponPickup_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
