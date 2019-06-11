VAL = 1

test = seq.program "seqtest" {
	state1=
	{
		seq.transition("VAL==1", function() VAL = 0; print("Hello,") end, "state2")
	},
	
	state2=
	{
		seq.transition("VAL==0", function() print("World") end, "exit"),
		seq.transition("VAL==1", function() print("Failure") end, "exit")
	}
}

test:run("state1")
