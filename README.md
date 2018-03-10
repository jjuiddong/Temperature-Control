# Temperature-Control
Temperature Control Using C++, VS2017, DX11, Imgui
- Input: AC 220V, 60Hz, Electric Current, Votage Phase, Temperature
- Output: Input Voltage On/Off


Input: AC 220V, 60Hz, Electric Current, Voltage Phase, Temperature

Output: Input Voltage On/Off



- display
	- Input Graph
		- Input Voltage, Current, Phase, Temperature(Virtual)

	- Output Graph
		- Voltage On/Off Switch


- Simulation
	- Registance
		- NTC registance change
			- change Temperature

		- Edit NTC Registance Graph
			- x axis : temperature
			- y axis : NTC Registance

	- Current
		- every 3 second

	- Temperature
		- virtual temperature simulation
			- calculate temperature using Output Electric Current
			- Edit Change Temperature Graph


- Algorithm
	- Temperature Control
		- Voltage Control
			- Phase Control


- Edit
	- NTC Registance Changes
		- temperature function

	- Temperature Changes
		- electronic current, area function

	- Input Voltage Changes
		- 220V, 60Hz
		- 220V, 50Hz
		- 110V, 60Hz
		- 110V, 50Hz


