#pragma once

#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"
#include "Json/Json.h"

namespace google{
	namespace protobuf{
		class GameProto
		{
			DescriptorPool pool;
			DynamicMessageFactory dmf;
			CSupportLoader& support;
		public:
			static void PbLogHandler(LogLevel level, const char* filename, int line,
				const string& message) {
					static const char* level_names[] = { "INFO", "WARNING", "ERROR", "FATAL" };

					// We use fprintf() instead of cerr because we want this to work at static
					// initialization time.
					fprintf(stderr, "libprotobuf %s %s:%d] %s\n",
						level_names[level], filename, line, message.c_str());
					fflush(stderr);  // Needed on MSVC.
			}
			GameProto(CSupportLoader& _support)
				:dmf(&pool)
				,support(_support)
			{
				SetLogHandler(PbLogHandler);
			}
			~GameProto()
			{
			}
			BOOL Init(const TCHAR * lpszDesc)
			{
				CVfsReader fr(support.GetSystem()->GetSysVfs(),lpszDesc,TRUE);
				IVFile * f = fr.GetFile();
				if(!f)
					return FALSE;

				const BYTE * pBuf = f->GetBuf();
				int nSize = f->GetSize();
				google::protobuf::FileDescriptorSet descriptorSet;
				if(descriptorSet.ParseFromArray(pBuf,nSize))
				{
					int nCount = descriptorSet.file_size();
					for(int i=0;i<nCount;i++)
					{
						const google::protobuf::FileDescriptorProto &fdp = descriptorSet.file(i);
						const std::string name = fdp.name();
						const google::protobuf::FileDescriptor * fd = pool.BuildFile(fdp);

						int messages = fd->message_type_count();
						for(int j=0;j<messages;j++)
						{
							const google::protobuf::Descriptor * desc = fd->message_type(j);
							const std::string sname = desc->full_name();
							desc = pool.FindMessageTypeByName(sname);
							assert(desc);
							const google::protobuf::Message * pMessage = dmf.GetPrototype(desc);
							assert(pMessage);

							int nFields = pMessage->GetDescriptor()->field_count();
							for(int x=0;x<nFields;x++)
							{
								const google::protobuf::FieldDescriptor * descField = pMessage->GetDescriptor()->field(x);
								const std::string fname = descField->full_name();
								std::string _name = descField->name();
								descField = NULL;
							}
						}
					}
				}
				return TRUE;
			}
			DescriptorPool * GetDescriptorPool(){return &pool;}
			DynamicMessageFactory * GetDynamicMessageFactory(){return &dmf;}
			Message * New(const std::string & name)
			{
				const google::protobuf::Descriptor * desc = pool.FindMessageTypeByName(name);
				if(desc == NULL)
					return NULL;
				return dmf.GetPrototype(desc)->New();
			}

			static void GameProto::Pb2Json(Json::Value& node,const google::protobuf::Message * pMessage)
			{
				assert(node.type() == Json::objectValue);
				int nFields = pMessage->GetDescriptor()->field_count();
				for(int i=0;i<nFields;i++)
				{
					const google::protobuf::FieldDescriptor * df = pMessage->GetDescriptor()->field(i);
					if(df->is_required() || df->is_optional())
					{
						if(df->is_optional() && !pMessage->GetReflection()->HasField(*pMessage,df))
						{
							continue;
						}
					
						switch(df->cpp_type())
						{
						case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
							{
								Json::Value new_node(Json::objectValue);
								const google::protobuf::Message& newMessage = pMessage->GetReflection()->GetMessage(*pMessage,df);
								Pb2Json(new_node,&newMessage);
								node[df->name()] = new_node;
							}
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
							node[df->name()] = Json::Value((Json::Value::Int)pMessage->GetReflection()->GetInt32(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
							node[df->name()] = Json::Value((Json::Value::Int64)pMessage->GetReflection()->GetInt64(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
							node[df->name()] = Json::Value((Json::Value::UInt)pMessage->GetReflection()->GetUInt32(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
							node[df->name()] = Json::Value((Json::Value::UInt64)pMessage->GetReflection()->GetInt64(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
							node[df->name()] = Json::Value(pMessage->GetReflection()->GetDouble(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
							node[df->name()] = Json::Value(pMessage->GetReflection()->GetFloat(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
							node[df->name()] = Json::Value(pMessage->GetReflection()->GetBool(*pMessage,df));
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
							node[df->name()] = Json::Value(pMessage->GetReflection()->GetEnum(*pMessage,df)->name());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
							node[df->name()] = Json::Value(pMessage->GetReflection()->GetString(*pMessage,df));
							break;
						}
					}
					else if(df->is_repeated())
					{
						Json::Value new_node(Json::arrayValue);	
						int nRepeated = pMessage->GetReflection()->FieldSize(*pMessage,df);
						for(int nIndex =0;nIndex<nRepeated;nIndex++)
						{
							switch(df->cpp_type())
							{
							case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
								{
									Json::Value new_element(Json::objectValue);
									const google::protobuf::Message& newMessage = pMessage->GetReflection()->GetRepeatedMessage(*pMessage,df,nIndex);
									Pb2Json(new_element,&newMessage);
									new_node.append(new_element);
								}
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
								new_node.append(Json::Value((Json::Value::Int)pMessage->GetReflection()->GetRepeatedInt32(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
								new_node.append(Json::Value((Json::Value::Int64)pMessage->GetReflection()->GetRepeatedInt64(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
								new_node.append(Json::Value((Json::Value::UInt)pMessage->GetReflection()->GetRepeatedUInt32(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
								new_node.append(Json::Value((Json::Value::UInt64)pMessage->GetReflection()->GetRepeatedInt64(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
								new_node.append(Json::Value(pMessage->GetReflection()->GetRepeatedDouble(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
								new_node.append(Json::Value(pMessage->GetReflection()->GetRepeatedFloat(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
								new_node.append(Json::Value(pMessage->GetReflection()->GetRepeatedBool(*pMessage,df,nIndex)));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
								new_node.append(Json::Value(pMessage->GetReflection()->GetRepeatedEnum(*pMessage,df,nIndex)->name()));
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
								new_node.append(Json::Value(pMessage->GetReflection()->GetRepeatedString(*pMessage,df,nIndex)));
								break;
							}
						}
						node[df->name()] = new_node;
					}
				}
			}

			static void GameProto::Json2Pb(google::protobuf::Message * pMessage,const Json::Value& node)
			{
				Json::Value defaultValue(Json::nullValue);	
				char szUtf8[256] = {0};

				assert(node.isObject());
				int nFields = pMessage->GetDescriptor()->field_count();
				for(int i=0;i<nFields;i++)
				{
					const google::protobuf::FieldDescriptor * df = pMessage->GetDescriptor()->field(i);
					Json::Value value = node.get(df->name(),defaultValue);

					if(df->is_required() || df->is_optional())
					{
						if(df->is_optional() && value.isNull())
							continue;
						assert(!value.isNull());
					
						switch(df->cpp_type())
						{
						case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
							{
								google::protobuf::Message * newMessage = pMessage->GetReflection()->MutableMessage(pMessage,df);
								Json2Pb(newMessage,value);
							}
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
							pMessage->GetReflection()->SetInt32(pMessage,df,value.asInt());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
							pMessage->GetReflection()->SetInt64(pMessage,df,value.asInt64());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
							pMessage->GetReflection()->SetUInt32(pMessage,df,value.asUInt());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
							pMessage->GetReflection()->SetUInt64(pMessage,df,value.asUInt64());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
							pMessage->GetReflection()->SetDouble(pMessage,df,value.asDouble());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
							pMessage->GetReflection()->SetFloat(pMessage,df,value.asFloat());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
							pMessage->GetReflection()->SetBool(pMessage,df,value.asBool());
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
							{
								const google::protobuf::EnumValueDescriptor * pvd = df->enum_type()->FindValueByName(value.asString());
								assert(pvd);
								pMessage->GetReflection()->SetEnum(pMessage,df, pvd);
							}
							break;
						case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
							pMessage->GetReflection()->SetString(pMessage,df,value.asString());	
							break;			
						}
					}
					else if(df->is_repeated())
					{
						assert(value.isArray());
						Json::Value::ArrayIndex nRepeated = (int)value.size();
						for(Json::Value::ArrayIndex nIndex =0;nIndex<nRepeated;nIndex++)
						{
							Json::Value array_value = value.get(nIndex,defaultValue);
							switch(df->cpp_type())
							{
							case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
								{
									google::protobuf::Message * newMessage = pMessage->GetReflection()->AddMessage(pMessage,df);
									Json2Pb(newMessage,array_value);
								}
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
								pMessage->GetReflection()->AddInt32(pMessage,df,array_value.asInt());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
								pMessage->GetReflection()->AddInt64(pMessage,df,array_value.asInt64());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
								pMessage->GetReflection()->AddUInt32(pMessage,df,array_value.asUInt());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
								pMessage->GetReflection()->AddUInt64(pMessage,df,array_value.asUInt64());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
								pMessage->GetReflection()->AddDouble(pMessage,df,array_value.asDouble());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
								pMessage->GetReflection()->AddFloat(pMessage,df,array_value.asFloat());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
								pMessage->GetReflection()->AddBool(pMessage,df,array_value.asBool());
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
								{
									const google::protobuf::EnumValueDescriptor * pvd = df->enum_type()->FindValueByName(array_value.asString());
									assert(pvd);
									pMessage->GetReflection()->AddEnum(pMessage,df, pvd);
								}
								break;
							case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
								pMessage->GetReflection()->AddString(pMessage,df,array_value.asString());	
								break;
							}
						}
					}
				}
			}
		};
	}
}