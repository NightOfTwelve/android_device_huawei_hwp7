## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := hwp7

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/huawei/hwp7/device_hwp7.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := hwp7
PRODUCT_NAME := cm_hwp7
PRODUCT_BRAND := huawei
PRODUCT_MODEL := hwp7
PRODUCT_MANUFACTURER := huawei
