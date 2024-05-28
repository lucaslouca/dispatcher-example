<template>
  <transition name="modal">
    <div class="modal-mask">
      <div class="modal-wrapper">
        <div class="modal-container">
          <SpinnerComp :color="`red`" />
          <h1>{{ message }}.</h1>
        </div>
      </div>
    </div>
  </transition>
</template>

<script setup lang="ts">
import SpinnerComp from '@/components/SpinnerComp.vue'

const props = defineProps<{
  message: string
}>()
</script>

<style scoped>
.modal-mask {
  position: fixed;
  z-index: 5000;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5);
  display: table;
  transition: opacity 0.3s ease;
}

.modal-wrapper {
  display: table-cell;
  vertical-align: middle;
}

.modal-container {
  width: 500px;
  margin: 0px auto;
  padding: 20px 30px;
  background-color: rgb(28, 34, 51);
  border-radius: 4px;
  box-shadow: 0px 1px 3px rgba(24, 0, 69, 0.1);
  transition: all 0.3s ease;
  color: ghostwhite;
}

h1 {
  text-align: center;
  margin-bottom: 0;
}

.modal-body {
  margin: 20px 0;
}

.modal-enter {
  opacity: 0;
}

.modal-leave-active {
  opacity: 0;
}

.modal-enter .modal-container,
.modal-leave-active .modal-container {
  -webkit-transform: scale(1.1);
  transform: scale(1.1);
}
</style>
