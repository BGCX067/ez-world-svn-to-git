protoc system.proto world_interface.proto world_template.proto world_data.proto equip.proto role.proto world_root.proto world_service.proto -o..\data\overlord.desc
protogen.exe ..\data\overlord.desc -output_directory=..\src\TestSuit\DotNetProtoTest
pause