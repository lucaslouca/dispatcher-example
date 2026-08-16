<template>
  <div class="spinner" :style="{ borderTopColor: color }">
    <div class="spinner-inner"></div>
  </div>
</template>

<script setup lang="ts">
interface Props {
  color?: string
}

withDefaults(defineProps<Props>(), {
  color: 'blue'
})
</script>

<style scoped>
.spinner {
  border: 4px solid #f3f3f3;
  border-top: 4px solid v-bind(color);
  border-radius: 50%;
  width: 40px;
  height: 40px;
  animation: spin 1s linear infinite;
  margin: 20px auto;
}

@keyframes spin {
  0% {
    transform: rotate(0deg);
  }
  100% {
    transform: rotate(360deg);
  }
}
</style>
