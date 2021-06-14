<Qucs Schematic 0.0.19>
<Properties>
  <View=0,0,800,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=Scheme2.dat>
  <DataDisplay=Scheme2.dpl>
  <OpenDisplay=1>
  <Script=Scheme2.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Назва>
  <FrameText1=Намалював:>
  <FrameText2=Дата:>
  <FrameText3=Версія:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <GND * 1 380 390 0 0 0 0>
  <R R1 1 520 110 -26 15 0 0 "1000 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <.DC DC1 1 130 100 0 63 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <Vdc V1 1 280 290 18 -26 0 1 "12 V" 1>
  <.SW SW1 1 130 310 0 63 0 0 "DC1" 1 "lin" 1 "VoltageVar" 1 "5 V" 1 "50 V" 1 "10" 1>
  <R R2 1 520 210 -26 15 0 0 "2000 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <IProbe Pr2 1 670 210 -26 16 0 0>
  <IProbe Pr1 1 670 110 -26 16 0 0>
</Components>
<Wires>
  <280 210 280 260 "" 0 0 0 "">
  <280 320 280 390 "" 0 0 0 "">
  <280 390 380 390 "" 0 0 0 "">
  <440 110 490 110 "" 0 0 0 "">
  <440 210 490 210 "" 0 0 0 "">
  <440 110 440 210 "" 0 0 0 "">
  <280 210 440 210 "" 0 0 0 "">
  <750 210 750 390 "" 0 0 0 "">
  <380 390 750 390 "" 0 0 0 "">
  <700 210 750 210 "" 0 0 0 "">
  <550 210 640 210 "" 0 0 0 "">
  <750 110 750 210 "" 0 0 0 "">
  <700 110 750 110 "" 0 0 0 "">
  <550 110 640 110 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
