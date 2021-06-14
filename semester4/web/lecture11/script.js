/* Helper function */

let formatCost = function(cost) {
	let hrn = Math.floor(cost);
	let kopiykas = Math.floor((cost - hrn) * 100);
	if (kopiykas < 10)
		kopiykas = "0" + kopiykas;

	return hrn + "," + kopiykas + " грн.";
}





let currentCountryInfo;

let prices = {
	japan: 5000,
	["weeb-museum"]: 500,
	hiroshima: 200,
	["tokyo-palace"]: 600,

	usa: 6000,
	["white-house"]: 400,
	rushmore: 200,
	canyon: 300,

	france: 4000,
	louvre: 1000,
	eiffel: 500,
	arc: 400,

	italy: 3000,
	["pisa-tower"]: 600,
	colosseum: 700,
	["sistine-chapel"]: 400,

	belarus: 50,
	belarus1: 1.50,
	belarus2: 2.50,
	belarus3: 3.50
};



function updateTotalCost() {
	const form = document.forms["travel-tickets"];

	let totalCost = 0;
	for (var ticket of form.elements) {
		if (ticket.checked)
			totalCost += prices[ticket.name];
	}

	console.log(totalCost);

	document.getElementById("total-price").textContent = formatCost(totalCost);
}



window.onload = function() {
	console.log("hello");

	let availableTickets = Object.keys(prices);
	for (let ticket of availableTickets) {
		console.log(ticket);
		let label = document.querySelector("label[for='" + ticket + "']");
		label.textContent += " (ціна: " + formatCost(prices[ticket]) + ")";
	}

	document.getElementById("country").addEventListener("change", function() {

		if (this.value.length > 0) {
			let countryInfo = document.getElementById(this.value + "-info");
			countryInfo.style.display = "block";

			if (currentCountryInfo)
				currentCountryInfo.style.display = "none";

			currentCountryInfo = countryInfo;
		}
	});


	const countryInfoDivs = document.getElementsByClassName("country-info");
	for (const country of countryInfoDivs) {

		const ticketOptionInputs = country.getElementsByTagName("input");
		//Disable all venue tickets if main ticket is disabled
		ticketOptionInputs[0].addEventListener("change", function() {
			if (!this.checked) {
				for (const ticketOptionInput of ticketOptionInputs)
					ticketOptionInput.checked = false;
			}
		});

		for (let i = 1; i < ticketOptionInputs.length; i++) {
			//Enable main ticket if venue ticket is enabled
			ticketOptionInputs[i].addEventListener("change", function() {
				if (this.checked)
					ticketOptionInputs[0].checked = true;
			});
		}
		
		for (const ticketOptionInput of ticketOptionInputs) {
			ticketOptionInput.addEventListener("change", updateTotalCost);
		}
	}
}
