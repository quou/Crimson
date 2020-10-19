local p = premake
local m = { }

-- Initialize extension
p.extensions.compiledb = m
m._VERSION             = p._VERSION

--
-- Create the Compile Database action
--

newaction {
	-- Metadata
	trigger     = 'compiledb',
	shortname   = 'Compile Database',
	description = 'Generate a compilation database',

	-- Capabilities
	valid_kinds = {
		'ConsoleApp',
		'WindowedApp',
		'StaticLib',
		'SharedLib',
		'Utility',
	},
	valid_languages = {
		'C',
		'C++',
	},
	valid_tools = {
		cc = {
			'clang',
			'gcc',
		}
	},

	-- Workspace generator
	onWorkspace = function( wks )
		p.generate( wks, 'compile_commands.json', m.compile_commands.generate )
	end,

	-- Project generator
	onProject = function( prj )
	end,
}

--
-- Decide when the full module should be loaded.
--

return function( cfg )
	return ( _ACTION == 'compiledb' )
end
