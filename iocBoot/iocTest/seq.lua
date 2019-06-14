local seq = {}

-- 
local function do_seq(mytable, start_state)
	local running_state = start_state
	
	repeat
		local state = mytable["states"][running_state]
		
		-- Check each transition
		for index = 1, #state do
			local transition = state[index]
			
			-- Call conditional code and evaluate as a boolean
			local condition_fulfilled = load("return (" .. transition["conditional"] .. ")")()
			
			if (condition_fulfilled) then
			
				-- Call any code the transition defines
				-- and allow it to modify the environment.
				-- Then, switch to the next state.
				load(transition["function"], "=(load)", "t", _ENV)()
				running_state = transition["next_state"]
				
				break
			end
		end
	until (running_state == "exit")
end

function seq.program(prog_name)
	local output = { program_name=prog_name }
	
	-- Allows the lua parser to chain space-separated data together
	local metatable = { __call = function(mytable, data)
		mytable.states = data
		return mytable
	end }
	
	setmetatable(output, metatable)
	
	output.run = do_seq;
	
	return output
end


-- All parameters are strings
function seq.transition(cond, func, nstate)
	local output = {}
	
	-- Code to evaluate to see if transition should happen
	output["conditional"] = cond
	
	-- Code to run if conditional is met
	output["function"] = func
	
	-- Name of next state to transition to
	output["next_state"] = nstate
	
	return output
end

return seq
