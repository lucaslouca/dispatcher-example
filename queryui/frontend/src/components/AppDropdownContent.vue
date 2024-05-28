<template>
  <div v-if="active">
    <slot />
  </div>
</template>

<script setup lang="ts">
import { inject, computed } from 'vue'
const sharedState = inject('sharedState') as any

const active = computed(() => {
  return sharedState.active
})
</script>

<style></style>
