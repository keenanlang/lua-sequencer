VAL = 1

test = seq.program "seqtest" {
	state1=
	{
		seq.transition("VAL==1", "VAL = 0; print('Hello,')", "state2")
	},
	
	state2=
	{
		seq.transition("VAL==0", "print('World')", "exit"),
		seq.transition("VAL==1", "print('Failure')", "exit")
	}
}

test:run("state1")
