<Qucs Schematic 0.0.19>
<Properties>
  <View=-70,-74,800,800,1,0,91>
  <Grid=10,10,1>
  <DataSet=Scheme1.dat>
  <DataDisplay=Scheme1.dpl>
  <OpenDisplay=1>
  <Script=Scheme1.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Назва>
  <FrameText1=Намалював:>
  <FrameText2=Дата:>
  <FrameText3=Версія:>
</Properties>
<Symbol>
  <.ID -20 -16 SUB>
  <Line -20 20 40 0 #000080 2 1>
  <Line 20 20 0 -40 #000080 2 1>
  <Line -20 -20 40 0 #000080 2 1>
  <Line -20 20 0 -40 #000080 2 1>
</Symbol>
<Components>
  <GND * 1 340 280 0 0 0 0>
  <.DC DC1 1 390 380 0 63 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <Vdc V1 1 260 250 18 -26 0 1 "VoltageVar" 1>
  <.SW SW1 1 90 370 0 63 0 0 "DC1" 1 "lin" 1 "VoltageVar" 1 "0 V" 1 "10 V" 1 "21" 0>
  <IProbe Pr1 1 530 180 -26 16 0 0>
  <R R1 1 360 180 -26 15 0 0 "2000 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
</Components>
<Wires>
  <260 280 340 280 "" 0 0 0 "">
  <260 180 260 220 "" 0 0 0 "">
  <260 180 330 180 "" 0 0 0 "">
  <340 280 620 280 "" 0 0 0 "">
  <620 180 620 280 "" 0 0 0 "">
  <560 180 620 180 "" 0 0 0 "">
  <390 180 500 180 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
