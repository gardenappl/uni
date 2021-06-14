let currentSliderImgNum = 1;
const sliderImgCount = 3;

function setSliderImg(imgNum) {
	let currentImg = document.getElementById("slider-img" + currentSliderImgNum);
	let newImg = document.getElementById("slider-img" + imgNum);

	currentImg.classList.remove("active");
	currentImg.classList.add("hidden");
	newImg.classList.remove("hidden");
	newImg.classList.add("active");
}

function hideFeedbackForm() {
	let feedbackElements = document.getElementsByClassName("feedback");
	for (let element of feedbackElements) {
		element.style.display = "none";
	}
	let contentWrapper = document.getElementById("non-footer-wrapper");
	contentWrapper.style["min-height"] = "";
}


window.addEventListener('DOMContentLoaded', function() {

	//update slider
	setInterval(function() {
		let newSliderImgNum = currentSliderImgNum + 1;
		if (newSliderImgNum > sliderImgCount)
			newSliderImgNum = 1;
		setSliderImg(newSliderImgNum);
		currentSliderImgNum = newSliderImgNum;
	}, 4000);

	document.getElementById("prev").addEventListener("click", function() {
		let newSliderImgNum = currentSliderImgNum - 1;
		if (newSliderImgNum <= 0)
			newSliderImgNum = sliderImgCount;
		setSliderImg(newSliderImgNum);
		currentSliderImgNum = newSliderImgNum;
	});

	document.getElementById("next").addEventListener("click", function() {
		let newSliderImgNum = currentSliderImgNum + 1;
		if (newSliderImgNum > sliderImgCount)
			newSliderImgNum = 1;
		setSliderImg(newSliderImgNum);
		currentSliderImgNum = newSliderImgNum;
	});

	document.getElementById("contact").addEventListener("click", function() {
		let feedbackElements = document.getElementsByClassName("feedback");
		for (let element of feedbackElements) {
			element.style.display = "block";
		}
		let wrapper = document.getElementById("non-footer-wrapper");
		wrapper.style["min-height"] = "1000px";
	});
	document.getElementById("feedback-close").addEventListener("click", hideFeedbackForm);
	document.getElementById("feedback-submit").addEventListener("click", function() {
		let form = document.getElementById("feedback-form");
		if (form.checkValidity())
			hideFeedbackForm();
	});

	let requiredFields = document.querySelectorAll("#feedback input[required]");
	for (let element of requiredFields) {
		element.addEventListener("invalid", function(event) {
			let validity = element.validity;
			if (validity.valueMissing) {
				element.placeholder = "Поле обязательно для заполнения";
				event.preventDefault();
			} else {
				element.placeholer = undefined;
			}
		});
	}
});
