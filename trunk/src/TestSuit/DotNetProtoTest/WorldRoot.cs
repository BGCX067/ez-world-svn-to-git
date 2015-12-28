// Generated by ProtoGen, Version=2.4.1.473, Culture=neutral, PublicKeyToken=55f7125234beb589.  DO NOT EDIT!
#pragma warning disable 1591, 0612
#region Designer generated code

using pb = global::Google.ProtocolBuffers;
using pbc = global::Google.ProtocolBuffers.Collections;
using pbd = global::Google.ProtocolBuffers.Descriptors;
using scg = global::System.Collections.Generic;
namespace world {
  
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
  [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
  [global::System.CodeDom.Compiler.GeneratedCodeAttribute("ProtoGen", "2.4.1.473")]
  public static partial class WorldRoot {
  
    #region Extension registration
    public static void RegisterAllExtensions(pb::ExtensionRegistry registry) {
    }
    #endregion
    #region Static variables
    internal static pbd::MessageDescriptor internal__static_world_Root__Descriptor;
    internal static pb::FieldAccess.FieldAccessorTable<global::world.Root, global::world.Root.Builder> internal__static_world_Root__FieldAccessorTable;
    #endregion
    #region Descriptor
    public static pbd::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbd::FileDescriptor descriptor;
    
    static WorldRoot() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          "ChB3b3JsZF9yb290LnByb3RvEgV3b3JsZBoUd29ybGRfdGVtcGxhdGUucHJv" + 
          "dG8aC2VxdWlwLnByb3RvGgpyb2xlLnByb3RvIogBCgRSb290EiwKCXRlbXBs" + 
          "YXRlcxgBIAIoCzIZLndvcmxkLnRlbXBsYXRlLlRlbXBsYXRlcxIpCgxlcXVp" + 
          "cF90YWJsZXMYAiACKAsyEy53b3JsZC5lcXVpcC5UYWJsZXMSJwoLcm9sZV90" + 
          "YWJsZXMYAyACKAsyEi53b3JsZC5yb2xlLlRhYmxlcw==");
      pbd::FileDescriptor.InternalDescriptorAssigner assigner = delegate(pbd::FileDescriptor root) {
        descriptor = root;
        internal__static_world_Root__Descriptor = Descriptor.MessageTypes[0];
        internal__static_world_Root__FieldAccessorTable = 
            new pb::FieldAccess.FieldAccessorTable<global::world.Root, global::world.Root.Builder>(internal__static_world_Root__Descriptor,
                new string[] { "Templates", "EquipTables", "RoleTables", });
        return null;
      };
      pbd::FileDescriptor.InternalBuildGeneratedFileFrom(descriptorData,
          new pbd::FileDescriptor[] {
          global::world.template.WorldTemplate.Descriptor, 
          global::world.equip.Equip.Descriptor, 
          global::world.role.Role.Descriptor, 
          }, assigner);
    }
    #endregion
    
  }
  #region Messages
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
  [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
  [global::System.CodeDom.Compiler.GeneratedCodeAttribute("ProtoGen", "2.4.1.473")]
  public sealed partial class Root : pb::GeneratedMessage<Root, Root.Builder> {
    private Root() { }
    private static readonly Root defaultInstance = new Root().MakeReadOnly();
    private static readonly string[] _rootFieldNames = new string[] { "equip_tables", "role_tables", "templates" };
    private static readonly uint[] _rootFieldTags = new uint[] { 18, 26, 10 };
    public static Root DefaultInstance {
      get { return defaultInstance; }
    }
    
    public override Root DefaultInstanceForType {
      get { return DefaultInstance; }
    }
    
    protected override Root ThisMessage {
      get { return this; }
    }
    
    public static pbd::MessageDescriptor Descriptor {
      get { return global::world.WorldRoot.internal__static_world_Root__Descriptor; }
    }
    
    protected override pb::FieldAccess.FieldAccessorTable<Root, Root.Builder> InternalFieldAccessors {
      get { return global::world.WorldRoot.internal__static_world_Root__FieldAccessorTable; }
    }
    
    public const int TemplatesFieldNumber = 1;
    private bool hasTemplates;
    private global::world.template.Templates templates_;
    public bool HasTemplates {
      get { return hasTemplates; }
    }
    public global::world.template.Templates Templates {
      get { return templates_ ?? global::world.template.Templates.DefaultInstance; }
    }
    
    public const int EquipTablesFieldNumber = 2;
    private bool hasEquipTables;
    private global::world.equip.Tables equipTables_;
    public bool HasEquipTables {
      get { return hasEquipTables; }
    }
    public global::world.equip.Tables EquipTables {
      get { return equipTables_ ?? global::world.equip.Tables.DefaultInstance; }
    }
    
    public const int RoleTablesFieldNumber = 3;
    private bool hasRoleTables;
    private global::world.role.Tables roleTables_;
    public bool HasRoleTables {
      get { return hasRoleTables; }
    }
    public global::world.role.Tables RoleTables {
      get { return roleTables_ ?? global::world.role.Tables.DefaultInstance; }
    }
    
    public override bool IsInitialized {
      get {
        if (!hasTemplates) return false;
        if (!hasEquipTables) return false;
        if (!hasRoleTables) return false;
        if (!Templates.IsInitialized) return false;
        if (!EquipTables.IsInitialized) return false;
        if (!RoleTables.IsInitialized) return false;
        return true;
      }
    }
    
    public override void WriteTo(pb::ICodedOutputStream output) {
      int size = SerializedSize;
      string[] field_names = _rootFieldNames;
      if (hasTemplates) {
        output.WriteMessage(1, field_names[2], Templates);
      }
      if (hasEquipTables) {
        output.WriteMessage(2, field_names[0], EquipTables);
      }
      if (hasRoleTables) {
        output.WriteMessage(3, field_names[1], RoleTables);
      }
      UnknownFields.WriteTo(output);
    }
    
    private int memoizedSerializedSize = -1;
    public override int SerializedSize {
      get {
        int size = memoizedSerializedSize;
        if (size != -1) return size;
        
        size = 0;
        if (hasTemplates) {
          size += pb::CodedOutputStream.ComputeMessageSize(1, Templates);
        }
        if (hasEquipTables) {
          size += pb::CodedOutputStream.ComputeMessageSize(2, EquipTables);
        }
        if (hasRoleTables) {
          size += pb::CodedOutputStream.ComputeMessageSize(3, RoleTables);
        }
        size += UnknownFields.SerializedSize;
        memoizedSerializedSize = size;
        return size;
      }
    }
    
    public static Root ParseFrom(pb::ByteString data) {
      return ((Builder) CreateBuilder().MergeFrom(data)).BuildParsed();
    }
    public static Root ParseFrom(pb::ByteString data, pb::ExtensionRegistry extensionRegistry) {
      return ((Builder) CreateBuilder().MergeFrom(data, extensionRegistry)).BuildParsed();
    }
    public static Root ParseFrom(byte[] data) {
      return ((Builder) CreateBuilder().MergeFrom(data)).BuildParsed();
    }
    public static Root ParseFrom(byte[] data, pb::ExtensionRegistry extensionRegistry) {
      return ((Builder) CreateBuilder().MergeFrom(data, extensionRegistry)).BuildParsed();
    }
    public static Root ParseFrom(global::System.IO.Stream input) {
      return ((Builder) CreateBuilder().MergeFrom(input)).BuildParsed();
    }
    public static Root ParseFrom(global::System.IO.Stream input, pb::ExtensionRegistry extensionRegistry) {
      return ((Builder) CreateBuilder().MergeFrom(input, extensionRegistry)).BuildParsed();
    }
    public static Root ParseDelimitedFrom(global::System.IO.Stream input) {
      return CreateBuilder().MergeDelimitedFrom(input).BuildParsed();
    }
    public static Root ParseDelimitedFrom(global::System.IO.Stream input, pb::ExtensionRegistry extensionRegistry) {
      return CreateBuilder().MergeDelimitedFrom(input, extensionRegistry).BuildParsed();
    }
    public static Root ParseFrom(pb::ICodedInputStream input) {
      return ((Builder) CreateBuilder().MergeFrom(input)).BuildParsed();
    }
    public static Root ParseFrom(pb::ICodedInputStream input, pb::ExtensionRegistry extensionRegistry) {
      return ((Builder) CreateBuilder().MergeFrom(input, extensionRegistry)).BuildParsed();
    }
    private Root MakeReadOnly() {
      return this;
    }
    
    public static Builder CreateBuilder() { return new Builder(); }
    public override Builder ToBuilder() { return CreateBuilder(this); }
    public override Builder CreateBuilderForType() { return new Builder(); }
    public static Builder CreateBuilder(Root prototype) {
      return new Builder(prototype);
    }
    
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("ProtoGen", "2.4.1.473")]
    public sealed partial class Builder : pb::GeneratedBuilder<Root, Builder> {
      protected override Builder ThisBuilder {
        get { return this; }
      }
      public Builder() {
        result = DefaultInstance;
        resultIsReadOnly = true;
      }
      internal Builder(Root cloneFrom) {
        result = cloneFrom;
        resultIsReadOnly = true;
      }
      
      private bool resultIsReadOnly;
      private Root result;
      
      private Root PrepareBuilder() {
        if (resultIsReadOnly) {
          Root original = result;
          result = new Root();
          resultIsReadOnly = false;
          MergeFrom(original);
        }
        return result;
      }
      
      public override bool IsInitialized {
        get { return result.IsInitialized; }
      }
      
      protected override Root MessageBeingBuilt {
        get { return PrepareBuilder(); }
      }
      
      public override Builder Clear() {
        result = DefaultInstance;
        resultIsReadOnly = true;
        return this;
      }
      
      public override Builder Clone() {
        if (resultIsReadOnly) {
          return new Builder(result);
        } else {
          return new Builder().MergeFrom(result);
        }
      }
      
      public override pbd::MessageDescriptor DescriptorForType {
        get { return global::world.Root.Descriptor; }
      }
      
      public override Root DefaultInstanceForType {
        get { return global::world.Root.DefaultInstance; }
      }
      
      public override Root BuildPartial() {
        if (resultIsReadOnly) {
          return result;
        }
        resultIsReadOnly = true;
        return result.MakeReadOnly();
      }
      
      public override Builder MergeFrom(pb::IMessage other) {
        if (other is Root) {
          return MergeFrom((Root) other);
        } else {
          base.MergeFrom(other);
          return this;
        }
      }
      
      public override Builder MergeFrom(Root other) {
        if (other == global::world.Root.DefaultInstance) return this;
        PrepareBuilder();
        if (other.HasTemplates) {
          MergeTemplates(other.Templates);
        }
        if (other.HasEquipTables) {
          MergeEquipTables(other.EquipTables);
        }
        if (other.HasRoleTables) {
          MergeRoleTables(other.RoleTables);
        }
        this.MergeUnknownFields(other.UnknownFields);
        return this;
      }
      
      public override Builder MergeFrom(pb::ICodedInputStream input) {
        return MergeFrom(input, pb::ExtensionRegistry.Empty);
      }
      
      public override Builder MergeFrom(pb::ICodedInputStream input, pb::ExtensionRegistry extensionRegistry) {
        PrepareBuilder();
        pb::UnknownFieldSet.Builder unknownFields = null;
        uint tag;
        string field_name;
        while (input.ReadTag(out tag, out field_name)) {
          if(tag == 0 && field_name != null) {
            int field_ordinal = global::System.Array.BinarySearch(_rootFieldNames, field_name, global::System.StringComparer.Ordinal);
            if(field_ordinal >= 0)
              tag = _rootFieldTags[field_ordinal];
            else {
              if (unknownFields == null) {
                unknownFields = pb::UnknownFieldSet.CreateBuilder(this.UnknownFields);
              }
              ParseUnknownField(input, unknownFields, extensionRegistry, tag, field_name);
              continue;
            }
          }
          switch (tag) {
            case 0: {
              throw pb::InvalidProtocolBufferException.InvalidTag();
            }
            default: {
              if (pb::WireFormat.IsEndGroupTag(tag)) {
                if (unknownFields != null) {
                  this.UnknownFields = unknownFields.Build();
                }
                return this;
              }
              if (unknownFields == null) {
                unknownFields = pb::UnknownFieldSet.CreateBuilder(this.UnknownFields);
              }
              ParseUnknownField(input, unknownFields, extensionRegistry, tag, field_name);
              break;
            }
            case 10: {
              global::world.template.Templates.Builder subBuilder = global::world.template.Templates.CreateBuilder();
              if (result.hasTemplates) {
                subBuilder.MergeFrom(Templates);
              }
              input.ReadMessage(subBuilder, extensionRegistry);
              Templates = subBuilder.BuildPartial();
              break;
            }
            case 18: {
              global::world.equip.Tables.Builder subBuilder = global::world.equip.Tables.CreateBuilder();
              if (result.hasEquipTables) {
                subBuilder.MergeFrom(EquipTables);
              }
              input.ReadMessage(subBuilder, extensionRegistry);
              EquipTables = subBuilder.BuildPartial();
              break;
            }
            case 26: {
              global::world.role.Tables.Builder subBuilder = global::world.role.Tables.CreateBuilder();
              if (result.hasRoleTables) {
                subBuilder.MergeFrom(RoleTables);
              }
              input.ReadMessage(subBuilder, extensionRegistry);
              RoleTables = subBuilder.BuildPartial();
              break;
            }
          }
        }
        
        if (unknownFields != null) {
          this.UnknownFields = unknownFields.Build();
        }
        return this;
      }
      
      
      public bool HasTemplates {
       get { return result.hasTemplates; }
      }
      public global::world.template.Templates Templates {
        get { return result.Templates; }
        set { SetTemplates(value); }
      }
      public Builder SetTemplates(global::world.template.Templates value) {
        pb::ThrowHelper.ThrowIfNull(value, "value");
        PrepareBuilder();
        result.hasTemplates = true;
        result.templates_ = value;
        return this;
      }
      public Builder SetTemplates(global::world.template.Templates.Builder builderForValue) {
        pb::ThrowHelper.ThrowIfNull(builderForValue, "builderForValue");
        PrepareBuilder();
        result.hasTemplates = true;
        result.templates_ = builderForValue.Build();
        return this;
      }
      public Builder MergeTemplates(global::world.template.Templates value) {
        pb::ThrowHelper.ThrowIfNull(value, "value");
        PrepareBuilder();
        if (result.hasTemplates &&
            result.templates_ != global::world.template.Templates.DefaultInstance) {
            result.templates_ = global::world.template.Templates.CreateBuilder(result.templates_).MergeFrom(value).BuildPartial();
        } else {
          result.templates_ = value;
        }
        result.hasTemplates = true;
        return this;
      }
      public Builder ClearTemplates() {
        PrepareBuilder();
        result.hasTemplates = false;
        result.templates_ = null;
        return this;
      }
      
      public bool HasEquipTables {
       get { return result.hasEquipTables; }
      }
      public global::world.equip.Tables EquipTables {
        get { return result.EquipTables; }
        set { SetEquipTables(value); }
      }
      public Builder SetEquipTables(global::world.equip.Tables value) {
        pb::ThrowHelper.ThrowIfNull(value, "value");
        PrepareBuilder();
        result.hasEquipTables = true;
        result.equipTables_ = value;
        return this;
      }
      public Builder SetEquipTables(global::world.equip.Tables.Builder builderForValue) {
        pb::ThrowHelper.ThrowIfNull(builderForValue, "builderForValue");
        PrepareBuilder();
        result.hasEquipTables = true;
        result.equipTables_ = builderForValue.Build();
        return this;
      }
      public Builder MergeEquipTables(global::world.equip.Tables value) {
        pb::ThrowHelper.ThrowIfNull(value, "value");
        PrepareBuilder();
        if (result.hasEquipTables &&
            result.equipTables_ != global::world.equip.Tables.DefaultInstance) {
            result.equipTables_ = global::world.equip.Tables.CreateBuilder(result.equipTables_).MergeFrom(value).BuildPartial();
        } else {
          result.equipTables_ = value;
        }
        result.hasEquipTables = true;
        return this;
      }
      public Builder ClearEquipTables() {
        PrepareBuilder();
        result.hasEquipTables = false;
        result.equipTables_ = null;
        return this;
      }
      
      public bool HasRoleTables {
       get { return result.hasRoleTables; }
      }
      public global::world.role.Tables RoleTables {
        get { return result.RoleTables; }
        set { SetRoleTables(value); }
      }
      public Builder SetRoleTables(global::world.role.Tables value) {
        pb::ThrowHelper.ThrowIfNull(value, "value");
        PrepareBuilder();
        result.hasRoleTables = true;
        result.roleTables_ = value;
        return this;
      }
      public Builder SetRoleTables(global::world.role.Tables.Builder builderForValue) {
        pb::ThrowHelper.ThrowIfNull(builderForValue, "builderForValue");
        PrepareBuilder();
        result.hasRoleTables = true;
        result.roleTables_ = builderForValue.Build();
        return this;
      }
      public Builder MergeRoleTables(global::world.role.Tables value) {
        pb::ThrowHelper.ThrowIfNull(value, "value");
        PrepareBuilder();
        if (result.hasRoleTables &&
            result.roleTables_ != global::world.role.Tables.DefaultInstance) {
            result.roleTables_ = global::world.role.Tables.CreateBuilder(result.roleTables_).MergeFrom(value).BuildPartial();
        } else {
          result.roleTables_ = value;
        }
        result.hasRoleTables = true;
        return this;
      }
      public Builder ClearRoleTables() {
        PrepareBuilder();
        result.hasRoleTables = false;
        result.roleTables_ = null;
        return this;
      }
    }
    static Root() {
      object.ReferenceEquals(global::world.WorldRoot.Descriptor, null);
    }
  }
  
  #endregion
  
}

#endregion Designer generated code