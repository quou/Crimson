# Behaviour Base Class

All script behaviours should inherit from this base class. This class cannot be instantiated.

`OnInit`, `OnUpdate`, `OnContactStay`, `OnContactEnter` and `OnContactExit` should be overridden to define behaviour. 

```cpp
abstract class CrimsonBehaviour {
	Entity m_entity;

	void Destroy();

	void OnInit();
	void OnUpdate(float delta);
	void OnContactStay(Entity other);
	void OnContactEnter(Entity other);
	void OnContactExit(Entity other);
}
```
