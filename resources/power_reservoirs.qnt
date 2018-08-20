{
"Reservoir": {
		"icon": {
			"filename": "reservoir.png" 
		},
		"Storage": {
			"type": "balance",		
			"massbalance": "true",
			"flow": "flow",
			"includeinoutput": "true",
			"description": "Initial Storage",
			"ask_user": "true"
		},
		"reservoir_power": {
			"type": "value",
			"default": "2",
			"description": "Reservoir rating curve power",
			"ask_user": "true"
		},
		"reservoir_coeff": {
			"type": "value",
			"default": "10",
			"description": "Reservoir rating curve coefficient",
			"ask_user": "true"
		}
	},
"Reservoir_link": {
		"icon": {
			"filename": "reservoir_link.png" 
		},
		"flow": {
			"type": "expression",		
			"expression": "reservoir_coeff.s*Storage.s^reservoir_power.s",
			"includeinoutput": "true",
			"applylimit": "true",
			"ask_user": "true"
		}
	},
"User": {
		"icon": {
			"filename": "farm.png" 
		},
		"Storage": {
			"type": "balance",		
			"massbalance": "true",
			"flow": "flow",
			"includeinoutput": "true",
			"description": "Initial Storage",
			"ask_user": "true"
		}
	},
"User_flow": {
		"icon": {
			"filename": "farm_link.png" 
		},
		"flow": {
			"type": "timeseries",		
			"includeinoutput": "true",
			"applylimit": "true",
			"description": "Flow rate time series",
			"ask_user": "true"
		}
	}
}


