#include "pch.h"
#include "Utils.h"
#include "Bootstrapper.h"

#using <Microsoft.Extensions.Configuration.dll>
#using <Microsoft.Extensions.Configuration.Abstractions.dll>
#using <Microsoft.Extensions.Configuration.FileExtensions.dll>
#using <Microsoft.Extensions.Configuration.Json.dll>

Bootstrapper::Bootstrapper(System::String^ basePath)
{
	_paths = gcnew System::Collections::Generic::List<System::String^>(10);
	_paths->Add(basePath);
	_paths->Add(System::IO::Path::Combine(basePath, "bin"));
}

Bootstrapper::~Bootstrapper()
{
	this->!Bootstrapper();
}

Bootstrapper::!Bootstrapper()
{
}

System::Reflection::Assembly^ Bootstrapper::LoadAssembly(System::String^ fileName)
{
	for each (System::String ^ path in _paths)
	{
		System::String^ filePath = System::IO::Path::Combine(path, fileName);

		if (System::IO::File::Exists(filePath))
		{
			return System::Reflection::Assembly::LoadFile(filePath);
		}
	}

	return nullptr;
}

Jacobi::Vst::Core::Plugin::IVstPluginCommandStub^ Bootstrapper::LoadManagedPlugin(System::String^ pluginPath)
{
	// create the plugin (command stub) factory
	auto factory = gcnew Jacobi::Vst::Core::Plugin::ManagedPluginFactory();

	// load the managed plugin assembly by its default name
	factory->LoadAssemblyByDefaultName(pluginPath);

	// create the managed type that implements the Plugin Command Stub interface (sends commands to plugin)
	Jacobi::Vst::Core::Plugin::IVstPluginCommandStub^ commandStub = factory->CreatePluginCommandStub();

	if (commandStub != nullptr)
	{
		auto basePath = System::IO::Path::GetDirectoryName(pluginPath);
		auto name = System::IO::Path::GetFileNameWithoutExtension(pluginPath);

		auto builder = gcnew Microsoft::Extensions::Configuration::ConfigurationBuilder();
		Microsoft::Extensions::Configuration::FileConfigurationExtensions::SetBasePath(builder, basePath);
		Microsoft::Extensions::Configuration::JsonConfigurationExtensions::AddJsonFile(
			builder, name + ".appsettings.json", /*optional*/ true);

		// assign config to commandStub
		commandStub->PluginConfiguration = builder->Build();
	}

	return commandStub;
}
