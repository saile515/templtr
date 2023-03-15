const navButton = document.getElementById("open-nav");

let isOpen = false;

navButton.onclick = () => {
	isOpen = !isOpen;
	if (isOpen) {
		document.querySelector("#navbar nav").classList.add("open");
	} else {
		document.querySelector("#navbar nav").classList.remove("open");
	}
};
