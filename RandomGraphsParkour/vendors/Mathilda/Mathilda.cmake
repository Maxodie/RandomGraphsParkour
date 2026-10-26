set(buildConfigSave ${CONFIG})
set(CONFIG "Dist")
add_subdirectory(RandomGraphsParkour/vendors/Mathilda/Mathilda)
set(CONFIG ${buildConfigSave})
