#pragma once
class SceneManager : public Singleton<SceneManager>
{
private:
	friend class Singleton;
	SceneManager();
	~SceneManager();
public:
	void Update();

	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

	void Start() {}
	void End() {}

	void Create(string key, Scene* scene);
	Scene* Add(string key);		//��ϵ� �� �� key�� �ش��ϴ� ���� curScenes�� ����
	void Remove(string key);	//curScenes�� key�� �ش��ϴ� ���� �ִٸ� ����

private:
	void AddScene();		//��ϵ� �� �� key�� �ش��ϴ� ���� curScenes�� ����
	void RemoveScene();	//curScenes�� key�� �ش��ϴ� ���� �ִٸ� ����

private:
	unordered_map<string, Scene*> scenes;
	list<Scene*> curScenes;

	vector<string> addScenes, removeScenes;
};
