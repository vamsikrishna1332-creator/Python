const menuToggle = document.querySelector('.menu-toggle');
const navLinks = document.querySelector('.nav-links');

menuToggle?.addEventListener('click', () => {
  navLinks.classList.toggle('open');
});

const tabs = document.querySelectorAll('.tab');
const panels = document.querySelectorAll('.tab-panel');

tabs.forEach((tab) => {
  tab.addEventListener('click', () => {
    tabs.forEach((t) => t.classList.remove('active'));
    panels.forEach((p) => p.classList.remove('active'));

    tab.classList.add('active');
    document.getElementById(tab.dataset.tab)?.classList.add('active');
  });
});

const slides = document.querySelectorAll('.project-card');
const next = document.querySelector('.carousel-btn.next');
const prev = document.querySelector('.carousel-btn.prev');
let currentSlide = 0;

function showSlide(index) {
  slides.forEach((slide) => slide.classList.remove('active-slide'));
  slides[index].classList.add('active-slide');
}

next?.addEventListener('click', () => {
  currentSlide = (currentSlide + 1) % slides.length;
  showSlide(currentSlide);
});

prev?.addEventListener('click', () => {
  currentSlide = (currentSlide - 1 + slides.length) % slides.length;
  showSlide(currentSlide);
});

const costForm = document.getElementById('cost-form');
const estimateOutput = document.getElementById('estimate-output');

const basePrice = {
  app: 22000,
  site: 9000,
  cloud: 16000,
};

const complexityMultiplier = {
  basic: 1,
  standard: 1.6,
  advanced: 2.3,
};

costForm?.addEventListener('submit', (e) => {
  e.preventDefault();
  const type = document.getElementById('project-type').value;
  const complexity = document.getElementById('complexity').value;
  const months = Number(document.getElementById('months').value);

  if (!type || !complexity || !months) {
    estimateOutput.textContent = 'Please fill all fields to get an estimate.';
    return;
  }

  const estimate = Math.round(basePrice[type] * complexityMultiplier[complexity] * (months / 4));
  estimateOutput.textContent = `Estimated budget: $${estimate.toLocaleString()} (USD). Exact pricing depends on scope, integrations, and support model.`;
});

const leadForm = document.getElementById('lead-form');
const formStatus = document.getElementById('form-status');

leadForm?.addEventListener('submit', (e) => {
  e.preventDefault();

  const name = document.getElementById('name').value.trim();
  const email = document.getElementById('email').value.trim();
  const service = document.getElementById('service').value.trim();
  const message = document.getElementById('message').value.trim();

  if (!name || !email || !service || !message) {
    formStatus.textContent = 'Please complete all required fields.';
    return;
  }

  formStatus.textContent = `Thanks ${name}! Your inquiry for ${service} was received. Our team will contact you at ${email} shortly.`;
  leadForm.reset();
});
