# Hinweise zur Entwicklung

## Erkennen von Memory Leaks

Lesestoff: [Was sind Memory Leaks und warum sind sie zu vermeiden?](https://learn.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library)

Es gibt im Moment keine schöne Möglichkeit, mit Bordmitteln bei der Ausführung der Unittests entstandene Memory Leaks auszugeben. Beispielsweise gibt es `_CrtDumpMemoryLeaks`, allerdings verträgt sich die Art und Weise, wie diese Funktion die Informationen ausgibt, nicht mit dem Test-Runner. Konkret ist das Problem, dass die Funktion richtigerweise zurückliefert, dass es Memory Leaks gab, allerdings tauchen sie in der Ausgabe nicht auf. Zudem hat dieser Ansatz den Nachteil, dass es auch nicht einfach ohne Verrenkungen lösbar ist, einzelne Tests fehlschlagen zu lassen.

Abhilfe schafft die Lösung, in sämtlichen Testklassen `setUp` und `tearDown`-Methoden zu ergänzen, welche vor und nach jedem einzelnen Test ausgeführt werden. Diese sehen mindestens wie folgt aus:
```cpp
TEST_METHOD_INITIALIZE(setUp)
{
	 _CrtMemCheckpoint(&this->previousState);
}

TEST_METHOD_CLEANUP(tearDown)
{
	_CrtMemState currentState, stateDiff;
	_CrtMemCheckpoint(&currentState);
	int diffResult = _CrtMemDifference(
		&stateDiff,
		&this->previousState,
		&currentState);
	if (diffResult) {
		Assert::Fail(L"memory leak(s) detected.");
	}
}
```

Zudem muss die Testklasse ein (privates) Attribut `_CrtMemState previousState` besitzen.

Um zu erkennen, wo genau ein Memory Leak sich befindet, helfen die folgenden Tools:
- "Diagnosetools" (Visual-Studio-Bordmittel)
- Erweiterungen, z. B. WonderLeak oder Deleaker
- Spezialprogramme wie valgrind oder Dr. Memory