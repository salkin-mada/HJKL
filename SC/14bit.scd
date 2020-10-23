// first

(
MIDIIn.connectAll;
20.collect({arg i, item; item = "cc%".format(i+1).asSymbol;}).do({arg paramName, i;
	var path = "_%".format(paramName).asSymbol;
	Ndef(path, 0.0).kr(1);
	FourteenBitCC.new("%_Responder".format(paramName).asSymbol, i+1, i+1+32).func = {|val| 
		var bits = 14;
		var	max = (2**bits-1);
		Ndef(path).bus.set(val.linlin(0,max,0.0,1.0));
		"%: %".format(paramName, val).postln;
	};
});
)

Ndef(\sov).free(1)
(
Ndef(\sov, {
	var amp= Ndef.kr('_cc5').linexp(0.0,1.0,0.0001,1.0).lag; 
	var spread= Ndef.kr('_cc10').lag;
	var center= Ndef.kr('_cc15').linlin(0.0,1.0,-1.0,1.0).lag;
	var delay = Ndef.kr('_cc20').lag;
	var freq1=Ndef.kr('_cc1').linexp(0.0,1.0,50,5e3).lag; var carfreq1=Ndef.kr('_cc2').linexp(0.0,1.0,50,15e3).lag;
	var freq2=Ndef.kr('_cc3').linexp(0.0,1.0,50,5e3).lag; var carfreq2=Ndef.kr('_cc4').linexp(0.0,1.0,50,15e3).lag;
	var freq3=Ndef.kr('_cc6').linexp(0.0,1.0,50,5e3).lag; var carfreq3=Ndef.kr('_cc7').linexp(0.0,1.0,50,15e3).lag;
	var freq4=Ndef.kr('_cc8').linexp(0.0,1.0,50,5e3).lag; var carfreq4=Ndef.kr('_cc9').linexp(0.0,1.0,50,15e3).lag;
	var freq5=Ndef.kr('_cc11').linexp(0.0,1.0,50,5e3).lag; var carfreq5=Ndef.kr('_cc12').linexp(0.0,1.0,50,15e3).lag;
	var freq6=Ndef.kr('_cc13').linexp(0.0,1.0,50,5e3).lag; var carfreq6=Ndef.kr('_cc14').linexp(0.0,1.0,50,15e3).lag;
	var freq7=Ndef.kr('_cc16').linexp(0.0,1.0,50,5e3).lag; var carfreq7=Ndef.kr('_cc17').linexp(0.0,1.0,50,15e3).lag;
	var freq8=Ndef.kr('_cc18').linexp(0.0,1.0,50,5e3).lag; var carfreq8=Ndef.kr('_cc19').linexp(0.0,1.0,50,15e3).lag;
	var osc1 = SinOsc.ar(freq1 * SinOsc.ar(carfreq1))*0.5;
	var osc2 = SinOsc.ar(freq2 * SinOsc.ar(carfreq2))*0.5;
	var osc3 = SinOsc.ar(freq3 * SinOsc.ar(carfreq3))*0.5;
	var osc4 = SinOsc.ar(freq4 * SinOsc.ar(carfreq4))*0.5;
	var osc5 = SinOsc.ar(freq5 * SinOsc.ar(carfreq5))*0.5;
	var osc6 = SinOsc.ar(freq6 * SinOsc.ar(carfreq6))*0.5;
	var osc7 = SinOsc.ar(freq7 * SinOsc.ar(carfreq7))*0.5;
	var osc8 = SinOsc.ar(freq8 * SinOsc.ar(carfreq8))*0.5;
	var sum;
	sum = Splay.ar([osc1,osc2,osc3,osc4,osc5,osc6,osc7,osc8], spread: spread, center: center) * amp;
	sum = sum + CombN.ar(sum, 0.5,0.1,10.0, mul: delay);
	sum = sum.flatten;
	Limiter.ar(LeakDC.ar(sum));
}).mold(2).fadeTime_(2).play;
)
