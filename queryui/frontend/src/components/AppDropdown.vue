<template>
  <div @click="toggle">
    <slot name="toggler">
      <button id="toggler">Toggle</button>
    </slot>
    <slot />
  </div>
</template>

<script setup lang="ts">
import { ref, provide, reactive } from 'vue'
import { loadState, getElement } from '@/common'

const sharedState = reactive({ active: false })
provide('sharedState', sharedState)

function toggle() {
  const closeListerner = (e: any) => {
    const dropdown = getElement('#toggler')
    if (catchOutsideClick(e, dropdown)) {
      window.removeEventListener('click', closeListerner), (sharedState.active = false)
    }
  }

  // Add event listener to watch clicks outside the menu
  window.addEventListener('click', closeListerner)

  sharedState.active = !sharedState.active
}

function catchOutsideClick(event: any, dropdown: any) {
  // When user clicks menu — do nothing
  if (dropdown == event.target) {
    return false
  }

  // When user clicks outside of the menu — close the menu
  if (sharedState.active && dropdown != event.target) {
    return true
  }
}
</script>

<style lang="scss" scoped></style>
