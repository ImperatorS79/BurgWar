RegisterClientScript()
RegisterClientAssets("rifle-30349_640.png")

ENTITY.IsNetworked = true
ENTITY.PlayerControlled = false
ENTITY.MaxHealth = 0

ENTITY.Properties = {}

function ENTITY:Initialize()
	self:SetCollider({ Collider = Circle(Vec2(0, 0), 128 * 0.3), IsTrigger = true })
	self:EnableCollisionCallbacks(true)

	if (CLIENT) then
		self:AddSprite({
			Scale = Vec2(0.3, 0.3),
			TexturePath = "rifle-30349_640.png"
		})
	end
end

function ENTITY:OnCollisionStart(other)
	if (SERVER and other.Name == "burger") then
		local owner = other:GetOwner()
		if (not owner:HasWeapon("weapon_rifle")) then
			owner:GiveWeapon("weapon_rifle")
			self:Kill()
			self.Parent:OnPowerupConsumed()
		end
	end

	return false
end

