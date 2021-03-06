ENTITY.IsNetworked = false

ENTITY.Properties = {
	{ Name = "size", Type = PropertyType.FloatSize },
}

function ENTITY:Initialize()
	local size = self:GetProperty("size")
	local colliderSize = size / 2
	self:SetCollider(Rect(-colliderSize, colliderSize))
	self:EnableCollisionCallbacks(true)

	if (EDITOR) then
		self:AddSprite({
			Color = { r = 0, g = 0, b = 0, a = 180 },
			RenderOrder = 2000,
			Size = self:GetProperty("size"),
		})
	end
end

function ENTITY:OnCollisionStart(other)
	other:Kill()
	return true
end

