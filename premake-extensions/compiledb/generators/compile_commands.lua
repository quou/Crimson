local p         = premake
local compiledb = p.extensions.compiledb
local m         = { }

compiledb.compile_commands = m

--
-- Generate the global 'compile_commands.json' file
--

function m.generate( wks )
	p.indent( '\t' )

	p.push( '[' )

	local should_pop = false

	for prj in p.workspace.eachproject( wks ) do
		local arbitrary_config = p.project.eachconfig( prj )()
		local files            = arbitrary_config.files

		for i=1,#files do
			local file = files[ i ]

			if( path.isnativefile( file ) ) then
				if( should_pop ) then p.pop( '},' ) end

				p.push( '{' )
				should_pop = true

				m.write_file( arbitrary_config, file )
			end
		end
	end

	p.pop( '}' )
	p.pop( ']' )
end

function m.write_file( cfg, filepath )
	local basename = path.getbasename( filepath )
	local output   = path.join( cfg.buildtarget.directory, basename..'.o' )

	p.w( '"file": "%s",', filepath )
	p.w( '"directory": "%s",', cfg.project.location )
	p.w( '"output": "%s",', output )
	m.write_arguments( cfg )
end

function m.write_arguments( cfg )
	local compiler      = iif( p.project.isc( cfg.project ), 'gcc', 'g++' )
	-- TODO: file configs may override flags
	local cxxflags      = p.tools.gcc.getcxxflags( cfg )
	local defines       = p.tools.gcc.getdefines( cfg.defines )
	local undefines     = p.tools.gcc.getundefines( cfg.undefines )
	local forceincludes = p.tools.gcc.getforceincludes( cfg )
	local includedirs   = p.tools.gcc.getincludedirs( cfg, cfg.includedirs, cfg.sysincludedirs )
	local ldflags       = p.tools.gcc.getldflags( cfg )
	local libdirs       = p.tools.gcc.getLibraryDirectories( cfg )
	local links         = p.tools.gcc.getlinks( cfg, false, false )
	local arguments     = table.unique( table.join( compiler, cxxflags, defines, undefines, forceincludes, includedirs, ldflags, libdirs, links ) )

	p.push( '"arguments": [' )

	for i=1,#arguments do
		local line = string.format( '"%s"', arguments[ i ] )

		-- Add comma for all lines except the last one
		if( i < #arguments ) then line = line..',' end

		p.w( '%s', line )
	end

	p.pop( ']' )
end
