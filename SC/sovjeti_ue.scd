(
fork{
	//free buffer if already allocated
	if(topEnvironment.includesKey('filee'), {
		topEnvironment.at('filee').free;
	});
	s.sync;

	// populate buffer
	topEnvironment.put(
		'filee', Buffer.read(s, "C:/someFolder/someFile.wav")
	);
	s.sync;

	"buffer loaded".postln;

	MIDIIn.connectAll;
	
	80.collect({arg i, item; item = "cc_%".format(i+1).asSymbol;}).do({arg paramName, i;
		var path = "/midi/%".format(paramName).asSymbol;
		Ndef(path, 0.0).kr(1);
		MIDIdef.cc("cc_%Responder".format(paramName).asSymbol, {arg val, num, chan, src;
			Ndef(path).bus.set(val);
		},
		ccNum: 1+i,
		chan: 0
		);
	});

	40.collect({arg i, item; item = "button_%".format(i+1).asSymbol;}).do({arg paramName, i;
		var value;
		var path = "/midi/%".format(paramName).asSymbol;
		Ndef(path, 0.0).kr(1);

		MIDIdef.noteOn("noteOn_%Responder".format(paramName).asSymbol, {arg velocity, note, chan, src;
			value = 1;
			Ndef(path).bus.set(value);
		},
		noteNum: 48+i,
		chan: 0
		);

		MIDIdef.noteOff("noteOff_%Responder".format(paramName).asSymbol, {arg velocity, note, chan, src;
			value = 0;
			Ndef(path).bus.set(value);
		},
		noteNum: 48+i,
		chan: 0
		);
	});

	Ndef(\sovjeti_ue, { | bufnum, numChannels |
		var sig, mod, player, env, sum;
		var rate = Ndef.kr('/midi/cc_1').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_6').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_11').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_16').linlin(0,127,0.0,3.0);
		var switch = Ndef.kr('/midi/button_6');
		var startPos =Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger = Impulse.kr(1.4)+Ndef.kr('/midi/cc_7').linlin(0,127,0.0,3.0);

		bufnum = topEnvironment['filee'].bufnum;
		numChannels = topEnvironment['filee'].numChannels;

		player=PlayBufCF.ar(
			numChannels,
			bufnum,
			BufRateScale.kr(bufnum)*rate,
			Impulse.kr(trspeed),//trigger,
			startPos, loop:1, lag: 0.3
		);

		sig = player.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sum = Limiter.ar(CompanderD.ar(Splay.ar(sig),0.5,1,1/3), 0.99, 0.00001);
		LeakDC.ar(sum);
	}).play;

	"sovjeti_ue is running".postln;
};
)








