#pragma once

/// <summary>
/// The Bootstrapper class loads the managed plugin assembly.
/// </summary>
ref class Bootstrapper
{
public:
	Bootstrapper(System::String^ basePath);
	~Bootstrapper();
	!Bootstrapper();

	static Jacobi::Vst::Core::Plugin::IVstPluginCommandStub^ LoadManagedPlugin(System::String^ pluginPath);

private:
	System::Reflection::Assembly^ LoadAssembly(System::String^ fileName);

	System::Collections::Generic::List<System::String^>^ _paths;
};
