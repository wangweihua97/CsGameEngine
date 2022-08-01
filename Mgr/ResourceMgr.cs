using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mgr;
using CsGameEngine.Struct;
using YamlDotNet.RepresentationModel;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace CsGameEngine.Mgr
{
    public interface LoadYaml
    {
        void Load(YamlMappingNode node);
    }
    public static class ResourceMgr
    {
        #region Scene
        public static DirectoryInfo SceneDir = new DirectoryInfo("Assert/Scenes");

        public static FileInfo CreatSceneFile(string name)
        {
            SceneStruct sceneStruct = new SceneStruct();
            sceneStruct.test.Add(100, 1000f);
            FileInfo fileInfo = new FileInfo(Path.Combine(SceneDir.FullName, name + ".scene"));
            sceneStruct.filePath = Path.Combine(SceneDir.FullName, name + ".scene");
            Serializer(sceneStruct, fileInfo.FullName);
            return fileInfo;
        }

        public static bool HaveSceneFile(string name)
        {
            SceneStruct sceneStruct = new SceneStruct();
            FileInfo fileInfo = new FileInfo(Path.Combine(SceneDir.FullName, name + ".scene"));
            return fileInfo.Exists;
        }

        public static SceneStruct GetScene(string fileName)
        {
            FileInfo fileInfo = new FileInfo(Path.Combine(SceneDir.FullName, fileName + ".scene"));
            SceneStruct sceneStruct = Deserializer<SceneStruct>( fileInfo.FullName);
            return sceneStruct;
        }

        public static void SaveScene(SceneStruct sceneStruct)
        {
            Serializer(sceneStruct, sceneStruct.filePath);
        }

        #endregion

        public static void Serializer<T>(T obj, string path)
        {
            StreamWriter yamlWriter = File.CreateText(path);
            //var ignore = new YamlIgnoreAttribute();
            var yamlSerializer = new SerializerBuilder().WithNamingConvention(CamelCaseNamingConvention.Instance)
                .Build();
            yamlSerializer.Serialize(yamlWriter, obj);
            yamlWriter.Close();
            EventCenterMgr.EventTrigger("ResChange");
        }

        public static T Deserializer<T>(string path)
        {
            StreamReader yamlReader = File.OpenText(path);
            var yamlDeserializer = new DeserializerBuilder()
                .WithNamingConvention(CamelCaseNamingConvention.Instance)
                .IgnoreUnmatchedProperties()
                .Build();
            T info = yamlDeserializer.Deserialize<T>(yamlReader);
            yamlReader.Close();
            return info;
        }

        public static T Deserializer<T>(string path , INodeTypeResolver nodeTypeResolver)
        {
            StreamReader yamlReader = File.OpenText(path);
            var yamlDeserializer = new DeserializerBuilder()
                .IgnoreUnmatchedProperties()
                .WithNamingConvention(CamelCaseNamingConvention.Instance)
                .WithNodeTypeResolver(nodeTypeResolver)
                .Build();
            T info = yamlDeserializer.Deserialize<T>(yamlReader);
            yamlReader.Close();
            return info;
        }

        public static T DeserializerByLoading<T>(string path) where T : LoadYaml ,new ()
        {
            StreamReader yamlReader = File.OpenText(path);
            var yaml = new YamlStream();
            yaml.Load(yamlReader);
            T t = new T();
            t.Load((YamlMappingNode)yaml.Documents[0].RootNode);
            return t;
        }
    }
}
